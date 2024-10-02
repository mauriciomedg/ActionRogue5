// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

ASItemChest::ASItemChest()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);

	Credit = 1.0f;

	// Tells Unreal to consider this actor to update that variables in the client
	bReplicates = true;
}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	PickUp(InstigatorPawn, TreasureCredit);

	bLidOpened = !bLidOpened;
	OnRep_LidOpened();
}

void ASItemChest::OnActorLoaded_Implementation()
{
	OnRep_LidOpened();
}

void ASItemChest::OnRep_LidOpened()
{
	float CurrPitch = bLidOpened ? TargetPitch : 0.0f;
	LidMesh->SetRelativeRotation(FRotator(CurrPitch, 0, 0));
}

void ASItemChest::GetLifetimeReplicatedProps(TArray< FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// set rules for replicated params
	// If the bLidOpened is changed in the server, send it to all the clients
	// This is a default rule. To be sent to all the clients. It can be another rule
	// that can be sent to an special client like a (hold a wearpon)

	DOREPLIFETIME(ASItemChest, bLidOpened);
}

