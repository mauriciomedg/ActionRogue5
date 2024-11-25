// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChaosModularVehicle/ModularVehicleClusterPawn.h"
#include "SModularVehicleClusterPawn.generated.h"

class ASSimModuleWheel;
/**
 * 
 */
UCLASS()
class ACTIONROGUE5_API ASModularVehicleClusterPawn : public AModularVehicleClusterPawn
{
	GENERATED_BODY()
	
public:
	//UPROPERTY(EditAnywhere, Category = "Attack")
	//TObjectPtr<ASSimModuleWheel> Wheel;
	
	virtual void BeginPlay() override;
};
