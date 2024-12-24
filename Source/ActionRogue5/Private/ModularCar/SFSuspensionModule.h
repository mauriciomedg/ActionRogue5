// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChaosModularVehicle/SuspensionSimModule.h"
#include "VehicleUtility.h"

struct Chaos::FAllInputs;
class Chaos::FSimModuleTree;
class Chaos::FClusterUnionPhysicsProxy;
class Chaos::FSuspensionConstraint;

/**
 * 
 */
class ACTIONROGUE5_API SFSuspensionModule : public FSuspensionSimModule
{

public:
	SFSuspensionModule(const FSuspensionSettings& Settings);
	~SFSuspensionModule();

	virtual void Simulate(float DeltaTime, const Chaos::FAllInputs& Inputs, Chaos::FSimModuleTree& VehicleModuleSystem) override;

};
