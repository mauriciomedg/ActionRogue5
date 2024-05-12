// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckLowHealth.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AI/SAICharacter.h"
#include "AIController.h"

void USBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();

	if (ensure(BlackBoardComp))
	{
		const AAIController* AIController = OwnerComp.GetAIOwner();

		if (ensure(AIController))
		{
			ASAICharacter* AIMinion = Cast<ASAICharacter>(AIController->GetPawn());

			if (AIMinion)
			{
				bool bHasLowHealth = (AIMinion->GetHealth() < 98);

				BlackBoardComp->SetValueAsBool(HasLowHealthKey.SelectedKeyName, bHasLowHealth);
			}
			

		}
	}
}
