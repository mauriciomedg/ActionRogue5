// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularCar/SModularVehicleBaseComponent.h"
#include "PhysicsProxy/ClusterUnionPhysicsProxy.h"
#include "ModularCar/SFSuspensionModule.h"

static TAutoConsoleVariable<float> CVarSuspensionMaxRaise(TEXT("su.SuspensionMaxRaise"), 10.0f, TEXT("Suspension Max Raise"), ECVF_Cheat);
static TAutoConsoleVariable<float> CVarSuspensionMaxDrop(TEXT("su.SuspensionMaxDrop"), 10.0f, TEXT("Suspension Max Drop"), ECVF_Cheat);
static TAutoConsoleVariable<float> CVarSpringRate(TEXT("su.SpringRate"), 1000.0f, TEXT("Spring Rate"), ECVF_Cheat);
static TAutoConsoleVariable<float> CVarSpringPreload(TEXT("su.SpringPreload"), 100.0f, TEXT("Spring Preload"), ECVF_Cheat);
static TAutoConsoleVariable<float> CVarSpringDamping(TEXT("su.SpringDamping"), 0.5f, TEXT("Spring Damping"), ECVF_Cheat);
static TAutoConsoleVariable<float> CVarSuspensionForceEffect(TEXT("su.SuspensionForceEffect"), 100000.0f, TEXT("Suspension Force Effect"), ECVF_Cheat);


USModularVehicleBaseComponent::USModularVehicleBaseComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void USModularVehicleBaseComponent::BeginPlay()
{
	Super::BeginPlay();
}

//#if WITH_EDITOR
//void USModularVehicleBaseComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
//{
//	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
//
//	if (PropertyName == GET_MEMBER_NAME_CHECKED(USModularVehicleBaseComponent, SuspensionMaxRaise))
//	{
//		UE_LOG(LogTemp, Log, TEXT("MyProperty has been changed in the editor to %d"), SuspensionMaxRaise);
//	}
//
//	Super::PostEditChangeProperty(PropertyChangedEvent);
//}
//#endif

void USModularVehicleBaseComponent::setValuesModularVehicle()
{
	Chaos::FClusterUnionPhysicsProxy* Proxy = static_cast<Chaos::FClusterUnionPhysicsProxy*>(GetPhysicsProxy());
	Chaos::FPBDRigidsSolver* Solver = Proxy->GetSolver<Chaos::FPBDRigidsSolver>();
	Solver->EnqueueCommandImmediate([Proxy, this]() mutable
		{
			if (VehicleSimulationPT)
			{
				TUniquePtr<Chaos::FSimModuleTree>& SimModuleTree = VehicleSimulationPT->AccessSimComponentTree();
				
				for (int I = 0; I < SimModuleTree->GetNumNodes(); I++)
				{
					if (Chaos::ISimulationModuleBase* Module = SimModuleTree->GetNode(I).SimModule)
					{
						if (Module->GetSimType() == Chaos::eSimType::Suspension)
						{
							SFSuspensionModule* ModuleSuspension = static_cast<SFSuspensionModule*>(Module);

							Chaos::FSuspensionSettings& Setup = ModuleSuspension->AccessSetup();
							
							Setup.MaxRaise = SuspensionMaxRaise;
							Setup.MaxDrop = SuspensionMaxDrop;
							Setup.SpringRate = SpringRate;  //Chaos::MToCm(SpringRate);
							Setup.SpringPreload = SpringPreload; // Chaos::MToCm(SpringPreload);
							Setup.SpringDamping = SpringDamping;
							Setup.SuspensionForceEffect = SuspensionForceEffect;
							Setup.MaxLength = FMath::Abs(SuspensionMaxRaise + SuspensionMaxDrop);

							FString String;
							Module->GetDebugString(String);
							UE_LOG(LogTemp, Warning, TEXT("..%s"), *String);
						}
						
					}
				}
			}
		});
}

void USModularVehicleBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (CVarSuspensionMaxRaise.GetValueOnGameThread() != SuspensionMaxRaise)
	{
		SuspensionMaxRaise = CVarSuspensionMaxRaise.GetValueOnGameThread();
		setValuesModularVehicle();
	} 
	else if (CVarSuspensionMaxDrop.GetValueOnGameThread() != SuspensionMaxDrop)
	{
		SuspensionMaxDrop = CVarSuspensionMaxDrop.GetValueOnGameThread();
		setValuesModularVehicle();
	}
	else if (CVarSpringRate.GetValueOnGameThread() != SpringRate)
	{
		SpringRate = CVarSpringRate.GetValueOnGameThread();
		setValuesModularVehicle();
	}
	else if (CVarSpringPreload.GetValueOnGameThread() != SpringPreload)
	{
		SpringPreload = CVarSpringPreload.GetValueOnGameThread();
		setValuesModularVehicle();
	}
	else if (CVarSpringDamping.GetValueOnGameThread() != SpringDamping)
	{
		SpringDamping = CVarSpringDamping.GetValueOnGameThread();
		setValuesModularVehicle();
	}
	else if (CVarSuspensionForceEffect.GetValueOnGameThread() != SuspensionForceEffect)
	{
		SuspensionForceEffect = CVarSuspensionForceEffect.GetValueOnGameThread();
		setValuesModularVehicle();
	}

}

