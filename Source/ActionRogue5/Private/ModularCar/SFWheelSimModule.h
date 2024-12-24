// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimModule/WheelModule.h"
#include "VehicleUtility.h"

class Chaos::FSimModuleTree;
/**
 * 
 */
class ACTIONROGUE5_API SFWheelSimModule : public Chaos::FWheelSimModule
{
	friend Chaos::FWheelOutputData;

public:
	SFWheelSimModule(const Chaos::FWheelSettings& Settings);
	~SFWheelSimModule();

	virtual void Simulate(float DeltaTime, const Chaos::FAllInputs& Inputs, Chaos::FSimModuleTree& VehicleModuleSystem) override;
	virtual void Animate(Chaos::FClusterUnionPhysicsProxy* Proxy);

	/** set wheel rotational speed to match the specified linear forwards speed */
	void SetLinearSpeed(float LinearMetersPerSecondIn)
	{
		SetAngularVelocity(LinearMetersPerSecondIn / Setup().Radius);
	}

private:

	float BrakeTorque;				// [N.m]

	FVector ForceFromFriction;
	float MassPerWheel;
	float SteerAngleDegrees;

	// for output
	bool bTouchingGround;
	float SlipAngle;
};
