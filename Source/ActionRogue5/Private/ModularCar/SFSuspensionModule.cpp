// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularCar/SFSuspensionModule.h"
#include "SimModule/SimModuleTree.h"
#include "ModularCar/SFWheelSimModule.h"
#include "Chaos/PBDSuspensionConstraints.h"
#include "PhysicsProxy/SuspensionConstraintProxy.h"
#include "PBDRigidsSolver.h"

using namespace Chaos;

SFSuspensionModule::SFSuspensionModule(const Chaos::FSuspensionSettings& Settings)
	: FSuspensionSimModule(Settings)
{
}

void SFSuspensionModule::Simulate(float DeltaTime, const FAllInputs& Inputs, Chaos::FSimModuleTree& VehicleModuleSystem)
{
	Chaos::FSuspensionSimModule::Simulate(DeltaTime, Inputs, VehicleModuleSystem);
}

SFSuspensionModule::~SFSuspensionModule()
{
}
