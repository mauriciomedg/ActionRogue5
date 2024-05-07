// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCreditChanged, int32, NewCredit, bool, bGrant);

class USSaveGame;
/**
 * 
 */
UCLASS()
class ACTIONROGUE5_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	float Credits = 0.0f;

public:

	virtual void OnRep_Score() override;
	
	UFUNCTION(BlueprintCallable)
	float GetCredits() const;

	UFUNCTION()
	void GrantCredits(int32 Credit);

	UFUNCTION()
	void CostCredits(int32 Cost);

	UPROPERTY(BlueprintAssignable)
	FOnCreditChanged OnCreditChange;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static ASPlayerState* GetSPlayerState(AActor* Player);

	//BlueprintNativeEvent, allows extends the function in blueprint
	UFUNCTION(BlueprintNativeEvent)
	void SavePlayerState(USSaveGame* SaveObject);

	UFUNCTION(BlueprintNativeEvent)
	void LoadPlayerState(USSaveGame* SaveObject);

};
