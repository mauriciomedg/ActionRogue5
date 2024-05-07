// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"
#include "SCharacter.h"
#include "SSaveGame.h"

void ASPlayerState::GrantCredits(int32 Credit)
{
	SetScore(GetScore() + Credit);
	OnRep_Score();
}

void ASPlayerState::CostCredits(int32 Cost)
{
	SetScore(GetScore() + Cost);
	OnRep_Score();
}

void ASPlayerState::OnRep_Score()
{
	Super::OnRep_Score();

	if (GetScore() > Credits)
	{
		OnCreditChange.Broadcast(GetScore(), true);
	}
	else
	{
		OnCreditChange.Broadcast(GetScore(), false);
	}

	Credits = GetScore();
}

float ASPlayerState::GetCredits() const
{
	return GetScore();
}

ASPlayerState* ASPlayerState::GetSPlayerState(AActor* Player)
{
	ACharacter* Charact = Cast<ACharacter>(Player);

	if (Charact)
	{
		return Cast<ASPlayerState>(Charact->GetPlayerState());
	}
	
	return nullptr;
}

void ASPlayerState::SavePlayerState_Implementation(USSaveGame* SaveObject)
{
	if (SaveObject)
	{
		SaveObject->Credits = GetScore();
	}
}

void ASPlayerState::LoadPlayerState_Implementation(USSaveGame* SaveObject)
{
	if (SaveObject)
	{
		GrantCredits(SaveObject->Credits);

		//SetScore(SaveObject->Credits);
		//Credits = SaveObject->Credits;
	}
}
