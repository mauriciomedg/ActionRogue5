// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "SVehicle.generated.h"

class USpringArmComponent;
class UCameraComponent;
/**
 * 
 */
UCLASS()
class ACTIONROGUE5_API ASVehicle : public AWheeledVehiclePawn
{
	GENERATED_BODY()

public:

	ASVehicle(const FObjectInitializer& ObjInit);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected: 

	virtual void PostInitializeComponents() override;

protected:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComp;

};
