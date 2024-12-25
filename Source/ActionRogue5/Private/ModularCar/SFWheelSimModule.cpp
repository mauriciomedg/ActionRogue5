// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularCar/SFWheelSimModule.h"

SFWheelSimModule::SFWheelSimModule(const Chaos::FWheelSettings& Settings)
	: Chaos::FWheelSimModule(Settings)
	, BrakeTorque(0.0f)
	, ForceFromFriction(FVector::ZeroVector)
	, MassPerWheel(500.0f)
	, SteerAngleDegrees(0.0f)
{
}

SFWheelSimModule::~SFWheelSimModule()
{
}

void SFWheelSimModule::Simulate(float DeltaTime, const Chaos::FAllInputs& Inputs, Chaos::FSimModuleTree& VehicleModuleSystem)
{
	float Re = Setup().Radius;
	float K = 0.4f;
	float TorqueScaling = 1.0f;
	float TractionControlAndABSScaling = 0.98f;	// how close to perfection is the system working

	float HandbrakeTorque = Setup().HandbrakeEnabled ? Inputs.GetControls().GetMagnitude(Chaos::HandbrakeControlName) * Setup().HandbrakeTorque : 0.0f;
	SteerAngleDegrees = Setup().SteeringEnabled ? Inputs.GetControls().GetMagnitude(Chaos::SteeringControlName) * Setup().MaxSteeringAngle : 0.0f;
	BrakeTorque = Inputs.GetControls().GetMagnitude(Chaos::BrakeControlName) * Setup().MaxBrakeTorque + HandbrakeTorque;
	LoadTorque = 0.0f;
	ForceFromFriction = FVector::ZeroVector;
	float TorqueFromGroundInteraction = 0.0f;
	float AvailableGrip = 0.0f;

	// TODO: think about doing this properly, stops vehicles rolling around on their own too much
	// i.e. an auto handbrake feature
	if (Setup().AutoHandbrakeEnabled && LocalLinearVelocity.X < Setup().AutoHandbrakeVelocityThreshold && (Inputs.GetControls().GetMagnitude(Chaos::BrakeControlName) < SMALL_NUMBER && Inputs.GetControls().GetMagnitude(Chaos::ThrottleControlName) < SMALL_NUMBER))
	{
		BrakeTorque = Setup().HandbrakeTorque;
	}

	bTouchingGround = GetForceIntoSurface() > SMALL_NUMBER;

	if (bTouchingGround)
	{
		FRotator SteeringRotator(0.f, SteerAngleDegrees, 0.f);
		FVector Vel = SteeringRotator.UnrotateVector(LocalLinearVelocity);
		FVector LocalWheelVelocity = (Setup().Axis == Chaos::EWheelAxis::X) ? FVector(Vel.X, Vel.Y, Vel.Z) : FVector(Vel.Y, Vel.X, Vel.Z); // Potential Axis Swap
		LocalWheelVelocity = Setup().ReverseDirection ? -LocalWheelVelocity : LocalWheelVelocity;

		float GroundAngularVelocity = LocalWheelVelocity.Y / Re;
		float Delta = GroundAngularVelocity - AngularVelocity;
		TorqueFromGroundInteraction = Delta * Setup().WheelInertia / DeltaTime; // torque from wheels moving over terrain

		// X is longitudinal direction, Y is lateral
		SlipAngle = Chaos::FVehicleUtility::CalculateSlipAngle(LocalWheelVelocity.Y, LocalWheelVelocity.X);

		float AppliedLinearDriveForce = DriveTorque / Re;
		float AppliedLinearBrakeForce = FMath::Abs(BrakeTorque) / Re;

		// Longitudinal multiplier now affecting both brake and steering equally
		AvailableGrip = GetForceIntoSurface() * GetSurfaceFriction() * Setup().FrictionMultiplier;

		float FinalLongitudinalForce = 0.f;
		float FinalLateralForce = 0.f;

		// currently just letting the brake override the throttle
		bool Braking = (BrakeTorque > FMath::Abs(DriveTorque));
		bool WheelLocked = false;

		// are we actually touching the ground
		if (GetForceIntoSurface() > SMALL_NUMBER)
		{
			// ABS limiting brake force to match force from the grip available
			if (Setup().ABSEnabled && Braking && FMath::Abs(AppliedLinearBrakeForce) > AvailableGrip)
			{
				if ((Braking && Setup().ABSEnabled) || (!Braking && Setup().TractionControlEnabled))
				{
					float Sign = (AppliedLinearBrakeForce > 0.0f) ? 1.0f : -1.0f;
					AppliedLinearBrakeForce = AvailableGrip * TractionControlAndABSScaling * Sign;
				}
			}

			// Traction control limiting drive force to match force from grip available
			if (Setup().TractionControlEnabled && !Braking && FMath::Abs(AppliedLinearDriveForce) > AvailableGrip)
			{
				float Sign = (AppliedLinearDriveForce > 0.0f) ? 1.0f : -1.0f;
				AppliedLinearDriveForce = AvailableGrip * TractionControlAndABSScaling * Sign;
			}

			if (Braking)
			{
				// whether the velocity is +ve or -ve when we brake we are slowing the vehicle down
				// so force is opposing current direction of travel.
				float ForceRequiredToBringToStop = MassPerWheel * K * (LocalWheelVelocity.X) / DeltaTime;
				FinalLongitudinalForce = AppliedLinearBrakeForce;

				// check we are not applying more force than required so we end up overshooting 
				// and accelerating in the opposite direction
				FinalLongitudinalForce = FMath::Clamp(FinalLongitudinalForce, -FMath::Abs(ForceRequiredToBringToStop), FMath::Abs(ForceRequiredToBringToStop));

				// ensure the brake opposes current direction of travel
				if (LocalWheelVelocity.X > 0.0f)
				{
					FinalLongitudinalForce = -FinalLongitudinalForce;
				}

			}
			else
			{
				FinalLongitudinalForce = AppliedLinearDriveForce;
			}

			float ForceRequiredToBringToStop = (MassPerWheel * K * LocalWheelVelocity.Y) / DeltaTime;

			// use slip angle to generate a sideways force
			if (Setup().LateralSlipGraph.IsEmpty())
			{
				float AngleLimit = FMath::DegreesToRadians(Setup().SlipAngleLimit);
				float ClippedAngle = FMath::Clamp(SlipAngle, -AngleLimit, AngleLimit);
				FinalLateralForce = FMath::Abs(SlipAngle) * Setup().CorneringStiffness;
			}
			else
			{
				FinalLateralForce = Setup().LateralSlipGraph.EvaluateY(FMath::RadiansToDegrees(SlipAngle)) * Setup().LateralSlipGraphMultiplier;
			}

			if (FinalLateralForce > FMath::Abs(ForceRequiredToBringToStop))
			{
				FinalLateralForce = FMath::Abs(ForceRequiredToBringToStop);
			}
			if (LocalWheelVelocity.Y > 0.0f)
			{
				FinalLateralForce = -FinalLateralForce;
			}

			float LengthSquared = FinalLongitudinalForce * FinalLongitudinalForce + FinalLateralForce * FinalLateralForce;
			bool bClipping = false;
			if (LengthSquared > 0.05f)
			{
				float Length = FMath::Sqrt(LengthSquared);

				float Clip = (AvailableGrip) / Length;
				if (Clip < 1.0f)
				{
					if (Braking /*&& !bEngineBraking*/)
					{
						WheelLocked = true;
					}

					bClipping = true;
					FinalLongitudinalForce *= Clip;
					FinalLateralForce *= Clip;

					// make the resulting forces less than ideal since there is slippage
					FinalLongitudinalForce *= Setup().SlipModifier;
					FinalLateralForce *= Setup().SlipModifier;

				}
			}

		}

		// Potential Axis Swap
		if (Setup().Axis == Chaos::EWheelAxis::X)
		{
			ForceFromFriction.X = FinalLongitudinalForce;
			ForceFromFriction.Y = FinalLateralForce;
		}
		else
		{
			check(Setup().Axis == Chaos::EWheelAxis::Y);
			ForceFromFriction.Y = FinalLongitudinalForce;
			ForceFromFriction.X = FinalLateralForce;
		}

		if (Setup().ReverseDirection)
		{
			ForceFromFriction = -ForceFromFriction;
		}

		AddLocalForce(SteeringRotator.RotateVector(ForceFromFriction));
	}

	TransmitTorque(VehicleModuleSystem, DriveTorque, BrakeTorque);

	DriveTorque -= AvailableGrip;
	if (DriveTorque < 0.0f)
	{
		DriveTorque = 0.0f;
	}

	DriveTorque *= TorqueScaling;

	BrakingTorque -= AvailableGrip;
	if (BrakingTorque < 0.0f)
	{
		BrakingTorque = 0.0f;
	}

	BrakingTorque *= TorqueScaling;

	LoadTorque = TorqueFromGroundInteraction;

	IntegrateAngularVelocity(DeltaTime, Setup().WheelInertia, Setup().MaxRotationVel);
}

void SFWheelSimModule::Animate(Chaos::FClusterUnionPhysicsProxy* Proxy)
{
	if (Proxy)
	{
		if (Chaos::FPBDRigidClusteredParticleHandle* ClusterChild = GetClusterParticle(Proxy))
		{
			float Direction = Setup().ReverseDirection ? -1.0f : 1.0f;
			FQuat Rot = (Setup().Axis == Chaos::EWheelAxis::Y) ? FQuat(FVector(1, 0, 0), -GetAngularPosition() * Direction) : FQuat(FVector(0, 1, 0), GetAngularPosition() * Direction);
			FQuat Steer = FQuat(FVector(0, 0, 1), FMath::DegreesToRadians(GetSteerAngleDegrees()));

			ClusterChild->ChildToParent().SetRotation(Steer * Rot * GetInitialParticleTransform().GetRotation());
		}
	}
}
