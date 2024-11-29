// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChaosModularVehicle/VehicleSimSuspensionComponent.h"
#include "SVehicleSimSuspensionComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (ModularVehicle), meta = (BlueprintSpawnableComponent), hidecategories = (Object, Replication, Cooking, Activation, LOD, Physics, Collision, AssetUserData, Event))
class ACTIONROGUE5_API USVehicleSimSuspensionComponent : public UVehicleSimSuspensionComponent
{
	GENERATED_BODY()
	
public:

	virtual Chaos::ISimulationModuleBase* CreateNewCoreModule() const override;
	
	
};
