// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularCar/SVehicleSimWheelComponent.h"
#include "ModularCar/SFWheelSimModule.h"

Chaos::ISimulationModuleBase* USVehicleSimWheelComponent::CreateNewCoreModule() const
{
	//return Super::CreateNewCoreModule();
	Chaos::FWheelSettings Settings;
	Settings.Radius = WheelRadius;
	Settings.Width = WheelWidth;
	Settings.WheelInertia = WheelInertia;
	Settings.FrictionMultiplier = FrictionMultiplier;
	Settings.CorneringStiffness = CorneringStiffness * 10000.0f;
	Settings.SlipAngleLimit = SlipAngleLimit;
	
	Settings.MaxBrakeTorque = Chaos::TorqueMToCm(MaxBrakeTorque);
	Settings.HandbrakeEnabled = bHandbrakeEnabled;
	Settings.HandbrakeTorque = Chaos::TorqueMToCm(HandbrakeTorque);
	Settings.SteeringEnabled = bSteeringEnabled;
	Settings.MaxSteeringAngle = bSteeringEnabled ? MaxSteeringAngle : 0.0f;
	Settings.ABSEnabled = bABSEnabled;
	Settings.TractionControlEnabled = bTractionControlEnabled;
	Settings.Axis = (Chaos::EWheelAxis)(AxisType);
	Settings.ReverseDirection = ReverseDirection;
	
	//Chaos::ISimulationModuleBase* Wheel = new Chaos::FWheelSimModule(Settings);
	Chaos::ISimulationModuleBase* Wheel = new SFWheelSimModule(Settings);
	Wheel->SetAnimationEnabled(bAnimationEnabled);
	
	return Wheel;
}
