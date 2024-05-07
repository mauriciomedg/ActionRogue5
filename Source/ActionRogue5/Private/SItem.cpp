// Fill out your copyright notice in the Description page of Project Settings.


#include "SItem.h"
#include "SPlayerState.h"

// Sets default values
ASItem::ASItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASItem::BeginPlay()
{
	Super::BeginPlay();
}

bool ASItem::PickUp(APawn* InstigatorPawn, int32 DeltaCredit)
{
	ASPlayerState* PayerState = ASPlayerState::GetSPlayerState(InstigatorPawn);

	if (PayerState)
	{
		if (DeltaCredit > 0)
		{
			PayerState->GrantCredits(DeltaCredit);
			return true;
		}
		
		if (PayerState->GetScore() + DeltaCredit > 0)
		{
			PayerState->CostCredits(DeltaCredit);
			return true;
		}
	}

	return false;
}

// Called every frame
void ASItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

