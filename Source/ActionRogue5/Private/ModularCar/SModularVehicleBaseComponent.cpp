// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularCar/SModularVehicleBaseComponent.h"
#include "PhysicsEngine/ClusterUnionComponent.h"
#include "PhysicsProxy/ClusterUnionPhysicsProxy.h"
#include "ModularCar/SFSuspensionModule.h"
#include "ModularCar/SFChassisModule.h"
#include "PBDRigidsSolver.h"

static TAutoConsoleVariable<float> CVarSuspensionMaxRaise(TEXT("su.SuspensionMaxRaise"), 10.0f, TEXT("Suspension Max Raise"), ECVF_Cheat);
static TAutoConsoleVariable<float> CVarSuspensionMaxDrop(TEXT("su.SuspensionMaxDrop"), 100.0f, TEXT("Suspension Max Drop"), ECVF_Cheat);
static TAutoConsoleVariable<float> CVarSpringRate(TEXT("su.SpringRate"), 1000.0f, TEXT("Spring Rate"), ECVF_Cheat);
static TAutoConsoleVariable<float> CVarSpringPreload(TEXT("su.SpringPreload"), 100.0f, TEXT("Spring Preload"), ECVF_Cheat);
static TAutoConsoleVariable<float> CVarSpringDamping(TEXT("su.SpringDamping"), 0.5f, TEXT("Spring Damping"), ECVF_Cheat);
static TAutoConsoleVariable<float> CVarSuspensionForceEffect(TEXT("su.SuspensionForceEffect"), 100000.0f, TEXT("Suspension Force Effect"), ECVF_Cheat);


USModularVehicleBaseComponent::USModularVehicleBaseComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, GravityDirection(0.0, 0.0, -980.0)
{
}

void USModularVehicleBaseComponent::BeginPlay()
{
	Super::BeginPlay();

	IConsoleManager& ConsoleManager = IConsoleManager::Get();
	ConsoleManager.FindConsoleVariable(TEXT("p.Chaos.Suspension.SlopeThreshold"))->Set(1, ECVF_SetByCode);
}

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

void USModularVehicleBaseComponent::SetGravity()
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
						if (Module->GetSimType() == Chaos::eSimType::Chassis)
						{
							SFChassisModule* ModuleSuspension = static_cast<SFChassisModule*>(Module);
	
							if (Chaos::FClusterUnionPhysicsProxy::FInternalParticle* ParentParticle = Proxy->GetParticle_Internal())
							{
								const FTransform BodyTransform(ParentParticle->GetR(), ParentParticle->GetX());

								auto InvM = Proxy->GetParticle_External()->InvM();

								float Mass = 1 / InvM;
								ModuleSuspension->Gravity = BodyTransform.InverseTransformVector(this->GravityDirection * Mass);
							}

							break;
						}
					}
				}
			}
		});
}

void USModularVehicleBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	SetGravity();

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

