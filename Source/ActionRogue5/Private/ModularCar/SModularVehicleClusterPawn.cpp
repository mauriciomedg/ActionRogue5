// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularCar/SModularVehicleClusterPawn.h"
#include "ModularCar/SVehicleSimWheelComponent.h"
#include "ModularCar/SVehicleSimSuspensionComponent.h"
//#include "GeometryCollection/GeometryCollectionComponent.h"
#include "ChaosModularVehicle/ClusterUnionVehicleComponent.h"

ASModularVehicleClusterPawn::ASModularVehicleClusterPawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ASModularVehicleClusterPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	GetComponents<USVehicleSimWheelComponent>(Wheels);
	GetComponents<USVehicleSimSuspensionComponent>(Suspensions);
}


void ASModularVehicleClusterPawn::BeginPlay()
{
	Super::BeginPlay();
}

