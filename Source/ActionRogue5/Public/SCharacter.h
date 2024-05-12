// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "SCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USInteractionComponent;
class UAnimMontage;
class USAttributeComponent;
class USActionComponent;
class USActionEffect;

UCLASS()
class ACTIONROGUE5_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	// VisibleAnywhere = read only, still useful to view in the editor and enforce a convention.
	UPROPERTY(VisibleAnywhere, Category = "Effect")
	FName HandSocketName;
	
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwingComp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;
	
public:
	// Sets default values for this character's properties
	ASCharacter();

protected:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr <USInteractionComponent> InteractionComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USActionComponent* ActionComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tag")
	FGameplayTag GrantBlackHoleAttackTag;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void SprintStart();
	void SprintStop();

	// Interaction
	void PrimaryInteract();

	// Attack
	void PrimaryAttackMagicProjectile();
	void PrimaryAttackBlackHole();
	void PrimaryAttackTeleport();
	
	virtual FVector GetPawnViewLocation() const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Works automatically just in the player controller, the character we are playing with, game mode, cheat manager, 
	UFUNCTION(Exec)
	void HealSelf(float Amount = 100.0f);
};
