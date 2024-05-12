// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_HealBack.h"

#include "GameFramework/Character.h"
#include "AIController.h"
#include "SAttributeComponent.h"

EBTNodeResult::Type USBTTask_HealBack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (ensure(AIController))
	{
		ACharacter* Minion = Cast<ACharacter>(AIController->GetPawn());

		if (!Minion)
		{
			return EBTNodeResult::Failed;
		}

		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(Minion->GetComponentByClass(USAttributeComponent::StaticClass()));

		if (AttributeComp)
		{
			if (AttributeComp->ApplyHealthChange(nullptr, 100.0f))
			{
				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;

}
