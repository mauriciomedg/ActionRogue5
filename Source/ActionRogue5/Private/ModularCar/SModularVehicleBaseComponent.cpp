// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularCar/SModularVehicleBaseComponent.h"
#include "PhysicsEngine/ClusterUnionComponent.h"
#include "PhysicsProxy/ClusterUnionPhysicsProxy.h"
#include "ModularCar/SFSuspensionModule.h"
#include "ModularCar/SFChassisModule.h"
#include "PBDRigidsSolver.h"

static TAutoConsoleVariable<bool> CVarSetAcc(TEXT("su.SetAcc"), false, TEXT("Suspension Max Raise"), ECVF_Cheat);
static TAutoConsoleVariable<float> CVarSuspensionMaxRaise(TEXT("su.SuspensionMaxRaise"), 10.0f, TEXT("Suspension Max Raise"), ECVF_Cheat);
static TAutoConsoleVariable<float> CVarSuspensionMaxDrop(TEXT("su.SuspensionMaxDrop"), 15.0f, TEXT("Suspension Max Drop"), ECVF_Cheat);
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

	//Chaos::FClusterUnionPhysicsProxy* Proxy = static_cast<Chaos::FClusterUnionPhysicsProxy*>(GetPhysicsProxy());
	//
	//if (Proxy)
	//{
	//	auto InvM = Proxy->GetParticle_External()->InvM();
	//	TotalMass = 1.0 / InvM;
	//}
	
	// This gives us access to the PT parent cluster and child particles
	
	
	//Chaos::FClusterUnionPhysicsProxy* Proxy = static_cast<Chaos::FClusterUnionPhysicsProxy*>(GetPhysicsProxy());
	//Chaos::FPBDRigidsSolver* Solver = Proxy->GetSolver<Chaos::FPBDRigidsSolver>();
	//
	//Chaos::FPBDRigidsEvolutionGBF& Evolution = *static_cast<Chaos::FPBDRigidsSolver*>(Proxy->GetSolver<Chaos::FPBDRigidsSolver>())->GetEvolution();
	//
	//Solver->EnqueueCommandImmediate([&]() mutable
	//	{
	//
	//		Chaos::FClusterUnionManager& ClusterUnionManager = Evolution.GetRigidClustering().GetClusterUnionManager();
	//		const Chaos::FClusterUnionIndex& CUI = Proxy->GetClusterUnionIndex();
	//		if (Chaos::FClusterUnion* ClusterUnion = ClusterUnionManager.FindClusterUnion(CUI))
	//		{
	//			Chaos::FPBDRigidClusteredParticleHandle* ClusterHandle = ClusterUnion->InternalCluster;
	//			TArray<Chaos::FPBDRigidParticleHandle*> Particles = ClusterUnion->ChildParticles;
	//			for (auto Particle : Particles)
	//			{
	//				TotalMass += 1.0 / Particle->InvM();
	//			}
	//
	//			TArray<Chaos::FPBDRigidClusteredParticleHandle*> Clusters;
	//		}
	//	});
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
						}
					}
				}
			}
		});
}

void USModularVehicleBaseComponent::setG()
{
	Chaos::FClusterUnionPhysicsProxy* Proxy = static_cast<Chaos::FClusterUnionPhysicsProxy*>(GetPhysicsProxy());
	Chaos::FPBDRigidsSolver* Solver = Proxy->GetSolver<Chaos::FPBDRigidsSolver>();
	
	Chaos::FPBDRigidsEvolutionGBF& Evolution = *static_cast<Chaos::FPBDRigidsSolver*>(Proxy->GetSolver<Chaos::FPBDRigidsSolver>())->GetEvolution();

	Solver->EnqueueCommandImmediate([&]() mutable
			{
	Evolution.AddForceFunction([&](Chaos::TTransientPBDRigidParticleHandle<Chaos::FReal, 3>& HandleIn, const Chaos::FReal Dt) {
			HandleIn.AddForce(FVector(0.0, 900.0, -980.f) / HandleIn.InvM());
			//Evolution.GetGravityForces().SetAcceleration(Chaos::FVec3(0, 100, 0.0), HandleIn.GravityGroupIndex());
		});
	
		});

	//Evolution.GetGravityForces().SetAcceleration(FVector(0.0, 900.0, -980.f), 0);


	//Solver->EnqueueCommandImmediate([&]() mutable
	//	{
	//		Chaos::FClusterUnionManager& ClusterUnionManager = Evolution.GetRigidClustering().GetClusterUnionManager();
	//		const Chaos::FClusterUnionIndex& CUI = Proxy->GetClusterUnionIndex();
	//		if (Chaos::FClusterUnion* ClusterUnion = ClusterUnionManager.FindClusterUnion(CUI))
	//		{
	//			TArray<Chaos::FPBDRigidParticleHandle*> Particles = ClusterUnion->ChildParticles;
	//			TUniquePtr<Chaos::FSimModuleTree>& SimModuleTree = VehicleSimulationPT->AccessSimComponentTree();
	//
	//			const TArray<Chaos::FSimModuleTree::FSimModuleNode>& ModuleArray = SimModuleTree->GetSimulationModuleTree();
	//			for (const Chaos::FSimModuleTree::FSimModuleNode& Node : ModuleArray)
	//			{
	//				if (Node.IsValid() && Node.SimModule && Node.SimModule->IsEnabled())
	//				{
	//					Chaos::FPBDRigidParticleHandle* Child = Node.SimModule->GetParticleFromUniqueIndex(Node.SimModule->GetParticleIndex().Idx, Particles);
	//					if (Child == nullptr)
	//					{
	//						continue;
	//					}
	//
	//					Evolution.GetGravityForces().SetAcceleration(Chaos::FVec3(0, 0, 9800), Child->GravityGroupIndex());
	//				}
	//			}
	//		}
	//	});

	//Solver->EnqueueCommandImmediate([&]() mutable
	//	{
	//		auto InternalParticle = Proxy->GetParticle_Internal();
	//		if (InternalParticle)
	//		{
	//			InternalParticle->ClusterIds();// SetLinearEtherDrag(LinearDamping);
	//			//Evolution->GetGravityForces().SetAcceleration(Chaos::FVec3(0, 0, 9800), InternalParticle->GravityGroupIndex());
	//		}
	//	});
	////////////////////////////////////////////////////////////////////////////////////////////
	//Chaos::FPBDRigidsEvolutionGBF* Evolution = Solver->GetEvolution();
	//
	//Solver->EnqueueCommandImmediate([&]() mutable
	//	{
	//		const TArray<Chaos::FPBDRigidParticleHandle*>& ParticlesActives = Evolution.GetParticles().GetActiveParticlesArray(); //GetActiveParticlesView();
	//		
	//		for (Chaos::FPBDRigidParticleHandle* PBDParticle : ParticlesActives)
	//		{
	//			auto& Particle = *PBDParticle;
	//			//Evolution->GetGravityForces().SetAcceleration(Chaos::FVec3(0, 0, 9800), Particle.GravityGroupIndex());
	//		}
	//	});


	
}

void USModularVehicleBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	SetGravity();

	if (SetAcc && CVarSetAcc.GetValueOnGameThread())
	{
		//setG();
		SetAcc = false;
	}
	
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

