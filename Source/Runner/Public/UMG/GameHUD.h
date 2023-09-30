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

protected:

	UFUNCTION()
	void OnPauseClick();

	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UPauseMenu> PauseMenuWidgetClass;
	UPROPERTY();
	UPauseMenu* PauseMenu;

public:

	UFUNCTION(BlueprintCallable)
	void InitializedHUD(ARunnerGameModeBase* GameMode);

	void SetCoinsCount(const int32 Coin);

	void SetLivesCount(int32 Lives);

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CoinsCount;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LivesCount;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* PauseButton;

	bool bPauseButtonClick = false;
};
