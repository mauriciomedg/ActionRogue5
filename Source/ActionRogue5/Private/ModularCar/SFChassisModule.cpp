// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularCar/SFChassisModule.h"
#include "SimModule/SimModuleTree.h"


SFChassisModule::SFChassisModule(const Chaos::FChassisSettings& Settings)
	: Chaos::FChassisSimModule(Settings)
{
}

SFChassisModule::~SFChassisModule()
{
}

void SFChassisModule::Simulate(float DeltaTime, const Chaos::FAllInputs& Inputs, Chaos::FSimModuleTree& VehicleModuleSystem)
{
	Chaos::FChassisSimModule::Simulate(DeltaTime, Inputs, VehicleModuleSystem);

	const FTransform& OffsetTransform = GetComponentTransform();
	Chaos::FChassisSimModule::AddLocalForce(OffsetTransform.InverseTransformVector(Gravity), true, true, false, FColor::Blue);
}
