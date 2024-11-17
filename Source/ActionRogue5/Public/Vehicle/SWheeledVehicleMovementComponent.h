// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "SWheeledVehicleMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUE5_API USWheeledVehicleMovementComponent : public UChaosWheeledVehicleMovementComponent
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "Game|Components|SWheeledVehicleMovement")
	void SetEngineStart(bool EngineStart);

	virtual void UpdateState(float DeltaTime) override;
	
	
};
