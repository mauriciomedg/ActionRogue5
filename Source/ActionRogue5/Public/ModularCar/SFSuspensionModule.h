// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimModule/SimModulesInclude.h"
#include "VehicleUtility.h"

struct Chaos::FAllInputs;
class Chaos::FSimModuleTree;
class Chaos::FClusterUnionPhysicsProxy;
class Chaos::FSuspensionConstraint;

/**
 * 
 */
class ACTIONROGUE5_API SFSuspensionModule : public Chaos::FSuspensionSimModule
{
	friend Chaos::FSuspensionSimModuleDatas;
	friend Chaos::FSuspensionOutputData;

public:
	SFSuspensionModule(const Chaos::FSuspensionSettings& Settings);
	~SFSuspensionModule();

	virtual bool IsBehaviourType(Chaos::eSimModuleTypeFlags InType) const override { return (InType & Chaos::Raycast); }


	virtual Chaos::FSimOutputData* GenerateOutputData() const override
	{
		return Chaos::FSuspensionOutputData::MakeNew();
	}

	virtual Chaos::eSimType GetSimType() const { return Chaos::eSimType::Suspension; }

	virtual const FString GetDebugName() const { return TEXT("Suspension"); }

	float GetSpringLength() const;
	void SetSpringLength(float InLength, float WheelRadius);
	virtual void GetWorldRaycastLocation(const FTransform& BodyTransform, float WheelRadius, Chaos::FSpringTrace& OutTrace);

	virtual void Simulate(float DeltaTime, const Chaos::FAllInputs& Inputs, Chaos::FSimModuleTree& VehicleModuleSystem) override;

	virtual void Animate(Chaos::FClusterUnionPhysicsProxy* Proxy) override;

	const FVector& GetRestLocation() const { return Setup().RestOffset; }

	void SetWheelSimTreeIndex(int WheelTreeIndexIn) { WheelSimTreeIndex = WheelTreeIndexIn; }
	virtual int GetWheelSimTreeIndex() const { return WheelSimTreeIndex; }

	void UpdateConstraint();

	void SetSuspensionConstraint(Chaos::FSuspensionConstraint* InConstraint);
	void SetConstraintIndex(int32 InConstraintIndex) { ConstraintIndex = InConstraintIndex; }
	int32 GetConstraintIndex() const { return ConstraintIndex; }
	void SetTargetPoint(const FVector& InTargetPoint, const FVector& InImpactNormal, bool InWheelInContact)
	{
		TargetPos = InTargetPoint;
		ImpactNormal = InImpactNormal;
		WheelInContact = InWheelInContact;
	}
};
