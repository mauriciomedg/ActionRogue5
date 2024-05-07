// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "STeleportDashProjectile.generated.h"

UCLASS()
class ACTIONROGUE5_API ASTeleportDashProjectile : public ASProjectileBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTeleportDashProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
protected:
	FTimerHandle TimerHandle_Teleport;

	////////////////////////////
	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float TeleportDelay = 0.2f;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float DetonateDelay = 0.2f;

	FTimerHandle TimerHandle_DelayedDetonate;

	// Base class using BlueprintNativeEvent, we must override the _implementation not the Explode()
	virtual void Explode_Implementation() override;

	void TeleportInstigator();
	////////////////////////////
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
