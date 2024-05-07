// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SItem.generated.h"

UCLASS()
class ACTIONROGUE5_API ASItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASItem();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual bool PickUp(APawn* InstigatorPawn, int32 DeltaCredit);

	UPROPERTY(VisibleAnywhere, Category = "Credit")
	int32 Credit = 0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
