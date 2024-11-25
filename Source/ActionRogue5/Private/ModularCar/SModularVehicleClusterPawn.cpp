// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularCar/SModularVehicleClusterPawn.h"
#include "ModularCar/SSimModuleWheel.h"
#include "ChaosModularVehicle/VehicleSimWheelComponent.h"
#include "ChaosModularVehicle/ModularVehicleBaseComponent.h"
#include "ChaosModularVehicle/ClusterUnionVehicleComponent.h"
#include "PhysicsEngine/ClusterUnionComponent.h"

void ASModularVehicleClusterPawn::BeginPlay()
{
	Super::BeginPlay();
	
	//if (Wheel && Wheel->SimComponent)
	//{
	//	TArray<FClusterUnionBoneData> BonesData;
	//	TArray<FClusterUnionBoneData> RemovedBoneIDs;
	//	TArray<int32> BoneIds;
	//
	//	ClusterUnionVehicleComponent->AddComponentToCluster(Cast<UPrimitiveComponent>(Wheel->MeshComp), BoneIds);
	//	VehicleSimComponent->AddComponentToSimulation(Cast<UPrimitiveComponent>(Wheel->SimComponent), BonesData, RemovedBoneIDs, true);
	//}
}

