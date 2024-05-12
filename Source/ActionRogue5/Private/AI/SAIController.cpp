// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"
#include "Kismet/GameplayStatics.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviorTree, TEXT("Behavior Tree is null!, Please assign BehaviorTree in your AI Controller")))
	{
		RunBehaviorTree(BehaviorTree);
	}
		
	// We replace this by the UPawnSensingComponent defined in the ASAICharacter

	//// we pass this as to tell the context, 0 is the player index
	//APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	//
	//if (MyPawn)
	//{
	//	//GetBlackboardComponent()->SetValueAsVector("MoveToLocation", MyPawn->GetActorLocation());
	//	GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn);
	//}
}