// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenu.generated.h"

/**
 * 
 */
UCLASS()
class RUNNER_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnContinueClick();

	UFUNCTION()
	void OnRestartClick();

	UFUNCTION()
	void OnQuitClick();
	
public:

	class UGameHUD* GameHUD;

	FORCEINLINE void SetGameHUD(UGameHUD* HUD) { GameHUD = HUD; }

	UPROPERTY(meta = (BindWidget))
	class UButton* ContinueButton;

	UPROPERTY(meta = (BindWidget))
	UButton* RestartButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;
};
