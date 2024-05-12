// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SInteractionComponent.h"
#include "SAttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "SActionComponent.h"
#include "SActionEffect.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	
	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");
	
	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChange.AddDynamic(this, &ASCharacter::OnHealthChanged);
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASCharacter::MoveForward(float Value)
{
	FRotator ControlRot(GetControlRotation());
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	AddMovementInput(ControlRot.Vector(), Value);
}

void ASCharacter::MoveRight(float Value)
{

	FRotator ControlRot(GetControlRotation());
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	//X = Forward (Red)
	//Y = Right (Green)
	//Z = Up (Blue)

	FVector RightVector(FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y));
	AddMovementInput(RightVector, Value);
}

void ASCharacter::SprintStart()
{
	ActionComp->StartActionByName(this, "Sprint");
}

void ASCharacter::SprintStop()
{
	ActionComp->StopActionByName(this, "Sprint");
}


void ASCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ASCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttackMagicProjectile);
	PlayerInputComponent->BindAction("PrimaryAttackBlackHole", IE_Pressed, this, &ASCharacter::PrimaryAttackBlackHole);
	PlayerInputComponent->BindAction("PrimaryAttackTeleport", IE_Pressed, this, &ASCharacter::PrimaryAttackTeleport);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASCharacter::SprintStop);
}

void ASCharacter::PrimaryAttackMagicProjectile()
{
	ActionComp->StartActionByName(this, "PrimaryAttack");
}

void ASCharacter::PrimaryAttackBlackHole()
{
	ActionComp->StartActionByName(this, "BlackHole");
}

void ASCharacter::PrimaryAttackTeleport()
{
	ActionComp->StartActionByName(this, "Dash");
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwingComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
	
	if (NewHealth < 50)
	{
		if (ActionComp->ActiveGameplayTags.HasTagExact(GrantBlackHoleAttackTag))
		{
			ActionComp->ActiveGameplayTags.RemoveTag(GrantBlackHoleAttackTag);
		}
	}
	else
	{
		if (!ActionComp->ActiveGameplayTags.HasTagExact(GrantBlackHoleAttackTag))
		{
			ActionComp->ActiveGameplayTags.AddTag(GrantBlackHoleAttackTag);
		}
	}
	
	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController* PlayerCtr = Cast<APlayerController>(GetController());
	
		DisableInput(PlayerCtr);
	
		SetLifeSpan(5.0f);
	}
}

void ASCharacter::HealSelf(float Amount /* - 100 */)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}

