// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicle/SVehicle.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Vehicle/SWheeledVehicleMovementComponent.h"
//#include "Vehicle/SChaosWheeledVehicleMovementComponent.h"
//#include "ChaosWheeledVehicleMovementComponent.h"
//#include "ChaosVehicleMovementComponent.h"

ASVehicle::ASVehicle(const FObjectInitializer& ObjInit)
	: Super(ObjInit.SetDefaultSubobjectClass<USWheeledVehicleMovementComponent>(TEXT("SWheeledVehicleComponent")))
{
}

void ASVehicle::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SpringArmComp = FindComponentByClass<USpringArmComponent>();
	CameraComp = FindComponentByClass<UCameraComponent>();
	
}

void ASVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//GetVehicleMovementComponent()->GetCurrentGear();
}