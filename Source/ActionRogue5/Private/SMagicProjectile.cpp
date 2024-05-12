// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"

//#include "Components/SphereComponent.h"
#include "SActionComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SGamePlayFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "SActionEffect.h"
//#include "Particles/ParticleSystemComponent.h"
//#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	//
	//// That is replaced by the profile preset "Projectile" created in the editor
	////SphereComp->SetCollisionObjectType(ECC_WorldDynamic);
	////SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	////SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	///////
	//
	//SphereComp->SetCollisionProfileName("Projectile");
	//RootComponent = SphereComp;
	//
	//EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	//EffectComp->SetupAttachment(SphereComp);
	//
	//MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;

	TimeProjectileAlive = 2.0f;

}

void ASMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
}

void ASMagicProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	OnActorResponse(HitComponent, OtherActor, OtherComp, Hit);
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnActorResponse(OverlappedComponent, OtherActor, OtherComp, SweepResult);
}

// Called when the game starts or when spawned
void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(TimerHandle_KillProjectile, this, &ASMagicProjectile::Explode, TimeProjectileAlive);
}

void ASMagicProjectile::OnActorResponse(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		USActionComponent* ActionComp = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));

		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			MovementComp->Velocity = -MovementComp->Velocity;
			SetInstigator(Cast<APawn>(OtherActor));

			return;
		}

		if (USGamePlayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, Hit))
		{
			GetWorldTimerManager().ClearTimer(TimerHandle_KillProjectile);

			Explode();

			if (ActionComp && HasAuthority())
			{
				//ActionComp->AddAction(GetInstigator(), BurningActionClass);
			}
		}

		APawn* InstigatorPawn = Cast<APawn>(OtherActor);
		if (InstigatorPawn)
		{
			APlayerController* PC = Cast<APlayerController>(InstigatorPawn->GetController());
			if (PC && PC->IsLocalController())
			{
				PC->ClientStartCameraShake(ImpactShake);
			}
		}
	}
}

// Called every frame
void ASMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SetTarget)
	{
		FVector Direction = Target - GetActorLocation();
		Direction.Normalize();

		MovementComp->AddForce(Direction * 10000);
	}
}

