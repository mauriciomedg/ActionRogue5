// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChaosModularVehicle/ModularVehicleBaseComponent.h"
#include "SModularVehicleBaseComponent.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUE5_API USModularVehicleBaseComponent : public UModularVehicleBaseComponent
{
	GENERATED_BODY()
	
protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	void setValuesModularVehicle();

public:
	
	USModularVehicleBaseComponent(const FObjectInitializer& ObjectInitializer);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
	UFUNCTION(BlueprintCallable)
	void SetGravity(const FVector& GravityAcceleration);

	UPROPERTY(VisibleAnywhere, Category = "FineTuneVehicle")
	float SuspensionMaxRaise = 5.0f;

	UPROPERTY(VisibleAnywhere, Category = "FineTuneVehicle")
	float SuspensionMaxDrop = 5.0f;
	
	UPROPERTY(VisibleAnywhere, Category = "FineTuneVehicle")
	float SpringRate = 100.0f;
	
	UPROPERTY(VisibleAnywhere, Category = "FineTuneVehicle")
	float SpringPreload = 50.0f;
	
	UPROPERTY(VisibleAnywhere, Category = "FineTuneVehicle")
	float SpringDamping = 0.9f;
	
	UPROPERTY(VisibleAnywhere, Category = "FineTuneVehicle")
	float SuspensionForceEffect = 100.0f;

};
