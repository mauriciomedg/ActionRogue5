// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChaosModularVehicle/SimModuleActor.h"
#include "SSimModuleChassis.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUE5_API ASSimModuleChassis : public ASimModuleActor
{
	GENERATED_BODY()
	
	virtual void PostInitializeComponents() override;

};
