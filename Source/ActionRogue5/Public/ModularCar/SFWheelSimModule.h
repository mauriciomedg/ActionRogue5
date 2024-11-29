// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimModule/SimModulesInclude.h"
#include "VehicleUtility.h"

/**
 * 
 */
class ACTIONROGUE5_API SFWheelSimModule : public Chaos::FWheelSimModule
{
public:
	SFWheelSimModule(const Chaos::FWheelSettings& Settings);
	virtual void Simulate(float DeltaTime, const Chaos::FAllInputs& Inputs, Chaos::FSimModuleTree& VehicleModuleSystem) override;

	void SetForceIntoSurface(float ForceIntoSurfaceIn) { ForceIntoSurface = ForceIntoSurfaceIn; }
	
	virtual bool IsBehaviourType(Chaos::eSimModuleTypeFlags InType) const override { return (InType & Chaos::TorqueBased) || (InType & Chaos::Velocity); }

	virtual float GetForceIntoSurface() const { return ForceIntoSurface; }
	virtual void SetSurfaceFriction(float FrictionIn) { SurfaceFriction = FrictionIn; }

	~SFWheelSimModule();

};
