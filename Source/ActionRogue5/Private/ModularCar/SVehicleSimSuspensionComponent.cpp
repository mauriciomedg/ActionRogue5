// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularCar/SVehicleSimSuspensionComponent.h"
#include "ModularCar/SFSuspensionModule.h"

using namespace Chaos;

Chaos::ISimulationModuleBase* USVehicleSimSuspensionComponent::CreateNewCoreModule() const
{
	// use the UE properties to setup the physics state
	FSuspensionSettings Settings;

	Settings.SuspensionAxis = SuspensionAxis;
	Settings.MaxRaise = SuspensionMaxRaise;
	Settings.MaxDrop = SuspensionMaxDrop;
	Settings.SpringRate = SpringRate; // Chaos::MToCm(SpringRate);
	Settings.SpringPreload = SpringPreload; // Chaos::MToCm(SpringPreload);
	Settings.SpringDamping = SpringDamping;
	Settings.SuspensionForceEffect = SuspensionForceEffect;

	//Settings.SwaybarEffect = SwaybarEffect;

	Chaos::ISimulationModuleBase* Suspension = new SFSuspensionModule(Settings);
	Suspension->SetAnimationEnabled(bAnimationEnabled);
	return Suspension;

};


