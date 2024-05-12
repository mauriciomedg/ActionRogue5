// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"
//#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "SAttributeComponent.h"
#include "EngineUtils.h" // for TActorIterator<>
#include "DrawDebugHelpers.h"
#include "SCharacter.h"
#include "SHealthPotion.h"
#include "SPlayerState.h"
#include "SSaveGame.h"
#include "Kismet/GameplayStatics.h"

// su. its to make the own category of console variables
// ECVF_Cheat means that it is not include in the final build
static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable spawning of bots via timer"), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.0f;

	PlayerStateClass = ASPlayerState::StaticClass();

	SlotName = "SaveGame01";
}

void ASGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	LoadSaveGame();
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBot, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);

}

void ASGameModeBase::SpawnBotTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot spawing disable via cvar 'CVarSpawnBots'."));
		return;
	}

	UEnvQueryInstanceBlueprintWrapper* QueryInst = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	// This is a better version of (GetAllActorsOfClassBy) 
	int32 NbOfAliveBots = 0;
	//for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	//{
	//	ASAICharacter* Bot = *It;
	//
	//	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);
	//
	//	if (ensure(AttributeComp) && AttributeComp->IsAlive())
	//	{
	//		NbOfAliveBots++;
	//	}
	//}

	UE_LOG(LogTemp, Warning, TEXT("Found %i alive bots."), NbOfAliveBots);

	float MaxBotCount = 10.0f;

	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->GetTimeSeconds());
	}

	if (NbOfAliveBots > MaxBotCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("At maximum bot capacity. Skipping bot spawn"));
		return;
	}

	//if (ensure(SpawnBotQuery))
	//{
	//	// Can run in multiple frames
	//	QueryInst->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);
	//}
}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.IsValidIndex(0))//Locations.Num() > 0)
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);

		DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false, 60.0f);
	}
}

void ASGameModeBase::OnQueryHealthPotionCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.IsValidIndex(0))//Locations.Num() > 0)
	{
		GetWorld()->SpawnActor<AActor>(HealthPotionClass, Locations[0], FRotator::ZeroRotator);

		DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false, 60.0f);
	}
}


void ASGameModeBase::KillAll()
{
	//for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	//{
	//	ASAICharacter* Bot = *It;
	//
	//	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);
	//
	//	if (ensure(AttributeComp) && AttributeComp->IsAlive())
	//	{
	//		AttributeComp->Kill(this); //@fixme: pass in player? for kill credit
	//	}
	//}
}

void ASGameModeBase::RespawnPlayerElapse(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();

		RestartPlayer(Controller);
	}
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	ASCharacter* Player = Cast<ASCharacter>(VictimActor);

	if (Player)
	{
		//It is important to use this handle as a local variable not to be override by another player that an die.
		//If it is reuse the SetTimer will be override the existing timer.
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapse", Player->GetController());

		float RespawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);

		UE_LOG(LogTemp, Log, TEXT("OnActorKilled: victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));
	}
	else
	{
		//ASAICharacter* Bot = Cast<ASAICharacter>(VictimActor);
		//
		//if (Bot)
		//{
		//	Player = Cast<ASCharacter>(Killer);
		//	if (Player)
		//	{
		//		ASPlayerState* PayerState = ASPlayerState::GetSPlayerState(Player);// Cast<ASPlayerState>(Player->GetPlayerState()); // GetSPlayerState();
		//		if (PayerState)
		//		{
		//			PayerState->GrantCredits(BotKilledCredit);
		//		}
		//	}
		//}
	}
}

void ASGameModeBase::OnHealthPotionPickUp()
{
	UEnvQueryInstanceBlueprintWrapper* QueryInst = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	// This is a better version of (GetAllActorsOfClassBy) 
	int32 NbHealthPotion = 0;
	for (TActorIterator<ASHealthPotion> It(GetWorld()); It; ++It)
	{
		ASHealthPotion* Item = *It;
		NbHealthPotion++;
	}

	UE_LOG(LogTemp, Warning, TEXT("Found %i HealthPotion."), NbHealthPotion);

	float MaxHealthPotion = 10.0f;

	if (NbHealthPotion > MaxHealthPotion)
	{
		UE_LOG(LogTemp, Warning, TEXT("At maximum bot capacity. Skipping bot spawn"));
		return;
	}

	if (ensure(SpawnBotQuery))
	{
		// Can run in multiple frames
		QueryInst->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryHealthPotionCompleted);
	}
}

void ASGameModeBase::WriteSaveGame()
{
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

void ASGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));

		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data"));
			return;
		}

		UE_LOG(LogTemp, Log, TEXT("Loaded SaveGame Data"));
	}
	else
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass()));
	}
}