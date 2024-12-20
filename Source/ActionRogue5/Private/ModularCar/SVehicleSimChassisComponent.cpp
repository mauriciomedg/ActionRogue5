// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularCar/SVehicleSimChassisComponent.h"
#include "ModularCar/SFChassisModule.h"

Chaos::ISimulationModuleBase* USVehicleSimChassisComponent::CreateNewCoreModule() const
{
	Chaos::FChassisSettings Settings;
	Settings.AreaMetresSquared = AreaMetresSquared;
	Settings.DragCoefficient = DragCoefficient;
	Settings.DensityOfMedium = DensityOfMedium;
	Settings.XAxisMultiplier = XAxisMultiplier;
	Settings.YAxisMultiplier = YAxisMultiplier;
	Settings.AngularDamping = AngularDamping;

	Chaos::ISimulationModuleBase* Chassis = new SFChassisModule(Settings);

	return Chassis;
}

