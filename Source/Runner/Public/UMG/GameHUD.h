// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameHUD.generated.h"

/**
 * 
 */
UCLASS()
class RUNNER_API UGameHUD : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void InitializedHUD(ARunnerGameModeBase* GameMode);

	void SetCoinsCount(const int32 Coin);

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CoinsCount;
	
};
