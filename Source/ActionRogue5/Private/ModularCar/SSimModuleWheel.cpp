// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularCar/SSimModuleWheel.h"
#include "Components/StaticMeshComponent.h"
#include "ChaosModularVehicle/VehicleSimWheelComponent.h"


void ASSimModuleWheel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MeshComp = FindComponentByClass<UStaticMeshComponent>();
	SimComponent = FindComponentByClass<UVehicleSimWheelComponent>();
}


