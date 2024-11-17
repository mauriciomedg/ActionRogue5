// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicle/SWheeledVehicleMovementComponent.h"

void USWheeledVehicleMovementComponent::SetEngineStart(bool EngineStart)
{
	if (PVehicleOutput)
	{
		FBodyInstance* TargetInstance = UpdatedPrimitive->GetBodyInstance();

		if (TargetInstance)
		{
			FPhysicsCommand::ExecuteWrite(TargetInstance->ActorHandle, [&](const FPhysicsActorHandle& Chassis)
				{
					if (VehicleSimulationPT && VehicleSimulationPT->PVehicle && VehicleSimulationPT->PVehicle->HasEngine())
					{
						if (EngineStart)
						{
							VehicleSimulationPT->PVehicle->GetEngine().StartEngine();
						}
						else
						{
							VehicleSimulationPT->PVehicle->GetEngine().StopEngine();
						}
					}
				});
		}
	}
}

void USWheeledVehicleMovementComponent::UpdateState(float DeltaTime)
{
	//Super::UpdateState(DeltaTime);

	// update input values
	AController* Controller = GetController();
	VehicleState.CaptureState(GetBodyInstance(), GetGravityZ(), DeltaTime);
	VehicleState.NumWheelsOnGround = 0;
	VehicleState.bVehicleInAir = false;
	int NumWheels = 0;
	if (PVehicleOutput)
	{
		for (int WheelIdx = 0; WheelIdx < PVehicleOutput->Wheels.Num(); WheelIdx++)
		{
			if (PVehicleOutput->Wheels[WheelIdx].InContact)
			{
				VehicleState.NumWheelsOnGround++;
			}
			else
			{
				VehicleState.bVehicleInAir = true;
			}
			NumWheels++;
		}
	}
	VehicleState.bAllWheelsOnGround = (VehicleState.NumWheelsOnGround == NumWheels);

	bool bProcessLocally = bRequiresControllerForInputs ? (Controller && Controller->IsLocalController()) : true;

	// IsLocallyControlled will fail if the owner is unpossessed (i.e. Controller == nullptr);
	// Should we remove input instead of relying on replicated state in that case?
	if (bProcessLocally && PVehicleOutput)
	{
		if (bReverseAsBrake)
		{
			//for reverse as state we want to automatically shift between reverse and first gear
			// Note: Removed this condition to support wheel spinning when rolling backwards with accelerator pressed, rather than braking
			//if (FMath::Abs(GetForwardSpeed()) < WrongDirectionThreshold)	//we only shift between reverse and first if the car is slow enough.
			{
				if (RawBrakeInput > KINDA_SMALL_NUMBER && GetCurrentGear() >= 0 && GetTargetGear() >= 0)
				{
					SetTargetGear(-1, true);
				}
				else if (RawThrottleInput > KINDA_SMALL_NUMBER && GetCurrentGear() <= 0 && GetTargetGear() <= 0)
				{
					SetTargetGear(1, true);
				}
			}
		}
		else
		{
			if (TransmissionType == Chaos::ETransmissionType::Automatic)
			{
				if (RawThrottleInput > KINDA_SMALL_NUMBER
					&& GetCurrentGear() == 0
					&& GetTargetGear() == 0)
				{
					SetTargetGear(1, true);
				}
			}

		}

		float ModifiedThrottle = 0.f;
		float ModifiedBrake = 0.f;
		CalcThrottleBrakeInput(ModifiedThrottle, ModifiedBrake);

		// Apply Inputs locally
		SteeringInput = SteeringInputRate.InterpInputValue(DeltaTime, SteeringInput, CalcSteeringInput());
		ThrottleInput = ThrottleInputRate.InterpInputValue(DeltaTime, ThrottleInput, ModifiedThrottle);
		BrakeInput = BrakeInputRate.InterpInputValue(DeltaTime, BrakeInput, ModifiedBrake);
		PitchInput = PitchInputRate.InterpInputValue(DeltaTime, PitchInput, CalcPitchInput());
		RollInput = RollInputRate.InterpInputValue(DeltaTime, RollInput, CalcRollInput());
		YawInput = YawInputRate.InterpInputValue(DeltaTime, YawInput, CalcYawInput());
		HandbrakeInput = HandbrakeInputRate.InterpInputValue(DeltaTime, HandbrakeInput, CalcHandbrakeInput());

		if (!bUsingNetworkPhysicsPrediction)
		{
			// and send to server - (ServerUpdateState_Implementation below)
			ServerUpdateState(SteeringInput, ThrottleInput, BrakeInput, HandbrakeInput, GetCurrentGear(), RollInput, PitchInput, YawInput);
		}

		if (PawnOwner && PawnOwner->IsNetMode(NM_Client))
		{
			MarkForClientCameraUpdate();
		}
	}
	else if (!bUsingNetworkPhysicsPrediction)
	{
		// use replicated values for remote pawns
		SteeringInput = ReplicatedState.SteeringInput;
		ThrottleInput = ReplicatedState.ThrottleInput;
		BrakeInput = ReplicatedState.BrakeInput;
		PitchInput = ReplicatedState.PitchInput;
		RollInput = ReplicatedState.RollInput;
		YawInput = ReplicatedState.YawInput;
		HandbrakeInput = ReplicatedState.HandbrakeInput;
		SetTargetGear(ReplicatedState.TargetGear, true);
	}
}


