// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SItem.h"
#include "SItemChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class ACTIONROGUE5_API ASItemChest : public ASItem, public ISGameplayInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	float TargetPitch = 110.0f;

	void Interact_Implementation(APawn* InstigatorPawn);
	void OnActorLoaded_Implementation();

	// Sets default values for this actor's properties
	ASItemChest();

protected:

	//
	UPROPERTY(ReplicatedUsing = "OnRep_LidOpened", BlueprintReadOnly, SaveGame) // ReplicatedUsing == RepNotif Knowing in doc and blueprint
	bool bLidOpened;

	UFUNCTION()
	void OnRep_LidOpened(); // this is only trigered automatically for clients. For the server we need to manually call it.

	UPROPERTY(EditAnywhere, Category = "Grant")
	int32 TreasureCredit = 1;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;
	
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;

};
