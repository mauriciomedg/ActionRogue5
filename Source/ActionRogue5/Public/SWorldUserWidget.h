// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SWorldUserWidget.generated.h"

class USizeBox;

/**
 * For things that are in the world and need to be projected in the 2D screen
 */
UCLASS()
class ACTIONROGUE5_API USWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	// Once in the UMG designer they created a widget with the same type and variable name, they will fill the pointer  
	UPROPERTY(meta = (BindWidget))
	USizeBox* ParentSizeBox;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:

	UPROPERTY(EditAnywhere, Category = "UI")
	FVector WorldOffset;

	//meta = (ExposeOnSpawn = true) information in the blueprint, that allows to show it as a pin in the widget.
	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (ExposeOnSpawn = true))
	AActor* AttachedActor;
};
