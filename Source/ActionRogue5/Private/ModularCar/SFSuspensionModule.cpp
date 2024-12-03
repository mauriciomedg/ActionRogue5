// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularCar/SFSuspensionModule.h"
#include "SimModule/SimModuleTree.h"
#include "ModularCar/SFWheelSimModule.h"
#include "Chaos/PBDSuspensionConstraints.h"
#include "PhysicsProxy/SuspensionConstraintProxy.h"
#include "PBDRigidsSolver.h"

using namespace Chaos;

SFSuspensionModule::SFSuspensionModule(const Chaos::FSuspensionSettings& Settings)
	: FSuspensionSimModule(Settings)
{
	AccessSetup().MaxLength = FMath::Abs(Settings.MaxRaise + Settings.MaxDrop);
}

float SFSuspensionModule::GetSpringLength() const
{
	return  -(Setup().MaxLength - SpringDisplacement);
}

void SFSuspensionModule::SetSpringLength(float InLength, float WheelRadius)
{
	float DisplacementInput = InLength;
	DisplacementInput = FMath::Max(0.f, DisplacementInput);
	SpringDisplacement = Setup().MaxLength - DisplacementInput;
}

void SFSuspensionModule::GetWorldRaycastLocation(const FTransform& BodyTransform, float WheelRadius, FSpringTrace& OutTrace)
{
	FVector LocalDirection = Setup().SuspensionAxis;
	FVector Local = GetParentRelativeTransform().GetLocation(); // change to just a vector and GetLocalLocation
	FVector WorldLocation = BodyTransform.TransformPosition(Local);
	FVector WorldDirection = BodyTransform.TransformVector(LocalDirection);

	OutTrace.Start = WorldLocation - WorldDirection * (Setup().MaxRaise);
	OutTrace.End = WorldLocation + WorldDirection * (Setup().MaxDrop + WheelRadius);
	float TraceLength = OutTrace.Start.Z - OutTrace.End.Z;
}

void SFSuspensionModule::UpdateConstraint()
{
	if (Constraint && Constraint->IsValid())
	{
		if (FSuspensionConstraintPhysicsProxy* Proxy = Constraint->GetProxy<FSuspensionConstraintPhysicsProxy>())
		{
			Chaos::FPhysicsSolver* Solver = Proxy->GetSolver<Chaos::FPhysicsSolver>();
			Solver->SetSuspensionTarget(Constraint, TargetPos, ImpactNormal, WheelInContact);
		}
	}
}

void SFSuspensionModule::SetSuspensionConstraint(FSuspensionConstraint* InConstraint)
{
	Constraint = InConstraint;
}

void SFSuspensionModule::Simulate(float DeltaTime, const FAllInputs& Inputs, FSimModuleTree& VehicleModuleSystem)
{
	{
		float ForceIntoSurface = 0.0f;
		if (SpringDisplacement > 0)
		{
			float Damping = Setup().SpringDamping;
			SpringSpeed = (LastDisplacement - SpringDisplacement) / DeltaTime;

			float StiffnessForce = SpringDisplacement * Setup().SpringRate;
			float DampingForce = SpringSpeed * Damping;
			float SuspensionForce = StiffnessForce - DampingForce;
			LastDisplacement = SpringDisplacement;

			if (SuspensionForce > 0)
			{
				ForceIntoSurface = SuspensionForce * Setup().SuspensionForceEffect;

				if (Constraint == nullptr)
				{
					AddLocalForce(Setup().SuspensionAxis * -SuspensionForce, true, false, true, FColor::Green);
				}
			}
		}

		// tell wheels how much they are being pressed into the ground
		if (SimModuleTree && WheelSimTreeIndex != INVALID_IDX)
		{
			if (Chaos::ISimulationModuleBase* Module = SimModuleTree->AccessSimModule(WheelSimTreeIndex))
			{
				check(Module->GetSimType() == eSimType::Wheel);
				SFWheelSimModule* Wheel = static_cast<SFWheelSimModule*>(Module);

				Wheel->SetForceIntoSurface(ForceIntoSurface);
			}

		}
	}

	if (Constraint)
	{
		UpdateConstraint();
	}
}

void SFSuspensionModule::Animate(FClusterUnionPhysicsProxy* Proxy)
{
	if (FPBDRigidClusteredParticleHandle* ClusterChild = GetClusterParticle(Proxy))
	{
		float CurrentSpringLength = GetSpringLength();

		FVector RestPos = GetInitialParticleTransform().GetTranslation();

		FVector Movement = Setup().SuspensionAxis * (Setup().MaxRaise + CurrentSpringLength);
		Movement = GetComponentTransform().TransformVector(Movement);
		FVector NewPos = RestPos - Movement;
		ClusterChild->ChildToParent().SetTranslation(NewPos); // local frame for module
	}
}

SFSuspensionModule::~SFSuspensionModule()
{
}
