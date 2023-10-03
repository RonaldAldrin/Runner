// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOver.generated.h"

/**
 * 
 */
UCLASS()
class RUNNER_API UGameOver : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnMainMenuClick();

	UFUNCTION()
	void OnRestartClick();

	UPROPERTY(meta = (BindWidget))
	class UButton* MainMenuButton;

	UPROPERTY(meta = (BindWidget))
	UButton* RestartButton;
public:

};
