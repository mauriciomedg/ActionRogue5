// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChaosModularVehicle/ModularVehicleClusterPawn.h"

#include "SModularVehicleClusterPawn.generated.h"

class USVehicleSimWheelComponent;
class USVehicleSimSuspensionComponent;
//class UGeometryCollectionComponent;
/**
 * 
 */
UCLASS()
class ACTIONROGUE5_API ASModularVehicleClusterPawn : public AModularVehicleClusterPawn
{
	GENERATED_BODY()
	
public:
	
	ASModularVehicleClusterPawn(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;

protected:

	virtual void PostInitializeComponents() override;


	UPROPERTY()
	TArray<USVehicleSimWheelComponent*> Wheels;

	UPROPERTY()
	TArray<USVehicleSimSuspensionComponent*> Suspensions;

	//UPROPERTY()
	//TArray<TObjectPtr<UGeometryCollectionComponent>> GeometryCollectionForWheels;
	
};
