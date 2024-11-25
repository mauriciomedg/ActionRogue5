// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChaosModularVehicle/SimModuleActor.h"
#include "SSimModuleWheel.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUE5_API ASSimModuleWheel : public ASimModuleActor
{
	GENERATED_BODY()
	

protected:

	virtual void PostInitializeComponents() override;
};
