// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularCar/SSimModuleChassis.h"
#include "Components/StaticMeshComponent.h"
#include "ChaosModularVehicle/VehicleSimChassisComponent.h"


void ASSimModuleChassis::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MeshComp = FindComponentByClass<UStaticMeshComponent>();
	SimComponent = FindComponentByClass<UVehicleSimChassisComponent>();
}


