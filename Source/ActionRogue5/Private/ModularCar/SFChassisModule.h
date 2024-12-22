// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimModule/ChassisModule.h"
#include "VehicleUtility.h"

struct Chaos::FAllInputs;
class Chaos::FSimModuleTree;
class Chaos::FClusterUnionPhysicsProxy;
class Chaos::FSuspensionConstraint;

/**
 * 
 */
class ACTIONROGUE5_API SFChassisModule : public Chaos::FChassisSimModule
{
public:
	SFChassisModule(const Chaos::FChassisSettings& Settings);
	~SFChassisModule();

	virtual void Simulate(float DeltaTime, const Chaos::FAllInputs& Inputs, Chaos::FSimModuleTree& VehicleModuleSystem) override;

	FVector Gravity;
};
