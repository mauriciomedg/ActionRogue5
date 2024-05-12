// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SItem.h"
#include "SHealthPotion.generated.h"

class UStaticMeshComponent;

UCLASS()
class ACTIONROGUE5_API ASHealthPotion : public ASItem, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASHealthPotion();

	void Interact_Implementation(APawn* InstigatorPawn);

protected:

	UPROPERTY(EditAnywhere, Category = "Cost")
	int32 PotionCost = 3;
	
	UPROPERTY(ReplicatedUsing = "OnRep_PotionTaken", BlueprintReadOnly) // ReplicatedUsing == RepNotif Knowing in doc and blueprint
	bool bPotionTaken;

	UFUNCTION()
	void OnRep_PotionTaken(); // this is only trigered automatically for clients. For the server we need to manually call it.

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BottleMesh;

	UFUNCTION()
	void EnableHealthPotion();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty>& OutLifetimeProps) const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
