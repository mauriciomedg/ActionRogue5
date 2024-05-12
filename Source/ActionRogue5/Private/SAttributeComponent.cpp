// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"
//#include "SGameModeBase.h"
#include "Net/UnrealNetwork.h"

// su. its to make the own category of console variables
// ECVF_Cheat means that it is not include in the final build
static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component"), ECVF_Cheat);

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	Health = HealthMax;

	// Because is a component, it needs to be called ..ByDefault(true) in the constructor
	SetIsReplicatedByDefault(true);
}

bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetMaxHealth());
}

float USAttributeComponent::GetMaxHealth() const
{
	return HealthMax;
}

float USAttributeComponent::GetHealth() const
{
	return Health;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

float USAttributeComponent::GetRage() const
{
	return Rage;
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f) // can be damage could be set in the console as God
	{
		return false;
	}

	if (Delta < 0.0f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();

		Delta *= DamageMultiplier;
	}

	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, HealthMax);
		
	float ActualDelta = NewHealth - OldHealth;

	// Is Server?
	if (GetOwner()->HasAuthority())
	{
		Health = NewHealth;

		if (ActualDelta != 0.0f)
		{
			// We can comment that becase if its called in the server, it will be called in its self and for the other clients. For the client side, this
			// is call locally.
			//OnHealthChange.Broadcast(InstigatorActor, this, Health, ActualDelta);
			MulticastHealthChanged(InstigatorActor, Health, ActualDelta);
		}

		// Dead
		if (ActualDelta < 0.0f && Health == 0.0f)
		{
			// Auth Authority, GameMode only exist in the sever.
			//ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();

			//if (GM)
			//{
			//	GM->OnActorKilled(GetOwner(), InstigatorActor);
			//}
		}

	}
	
	if (ActualDelta < 0.0f)
	{
		ApplyRage(InstigatorActor, -ActualDelta);
	}
	
	return ActualDelta != 0.0f;
}

bool USAttributeComponent::ApplyRage(AActor* InstigatorActor, float Delta)
{
	float OldRage = Rage;

	Rage = FMath::Clamp(Rage + Delta, 0.0f, RageMax);

	float ActualDelta = Rage - OldRage;

	if (ActualDelta != 0.0f)
	{
		MulticastRageChanged(InstigatorActor, Rage, ActualDelta);
	}
	
	return ActualDelta != 0.0f;
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return FromActor->FindComponentByClass<USAttributeComponent>();
		//return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}

	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* FromActor)
{
	USAttributeComponent* AttributeComp = GetAttributes(FromActor);

	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}

	return false;
}

void USAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewValue, float Delta)
{
	OnHealthChange.Broadcast(InstigatorActor, this, NewValue, Delta);
}

void USAttributeComponent::MulticastRageChanged_Implementation(AActor* InstigatorActor, float NewValue, float Delta)
{
	OnRageChange.Broadcast(InstigatorActor, this, NewValue, Delta);
}

void USAttributeComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// set rules for replicated params
	// If the bLidOpened is changed in the server, send it to all the clients
	// This is a default rule. To be sent to all the clients. It can be another rule
	// that can be sent to an special client like a (hold a wearpon)

	DOREPLIFETIME(USAttributeComponent, Health);
	DOREPLIFETIME(USAttributeComponent, HealthMax);
	DOREPLIFETIME(USAttributeComponent, Rage);
	DOREPLIFETIME(USAttributeComponent, RageMax);

	//COND_OwnerOnly: means only the owner see the variable updated if it changes by gameplay example. 
	//COND_InitialOnly: only when we spawn that player, we send it once.
	//DOREPLIFETIME_CONDITION(USAttributeComponent, HealthMax, COND_OwnerOnly);
}