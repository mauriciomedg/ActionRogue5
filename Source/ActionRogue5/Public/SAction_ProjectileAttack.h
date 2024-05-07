// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SAction_ProjectileAttack.generated.h"

class UAnimMontage;
class UParticleSystem;
class UCameraShakeBase;
/**
 * 
 */
UCLASS()
class ACTIONROGUE5_API USAction_ProjectileAttack : public USAction
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(VisibleAnywhere, Category = "Effect")
	FName HandSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackAnimDelay;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UParticleSystem* CastingEffect;

	UPROPERTY(EditAnywhere, Category = "Effect")
	TSubclassOf<class UCameraShakeBase> ShakeCamera;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	float InnerRadiusCameraShake = 1.0;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	float OuterRadiusCameraShake = 2.0;

	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);

public:

	virtual void StartAction_Implementation(AActor* Instigator) override;
	
	USAction_ProjectileAttack();
	


};
