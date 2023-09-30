// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG/GameHUD.h"
#include "Components/TextBlock.h"
#include "GameMode/RunnerGameModeBase.h"
#include "Components/Button.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UMG/PauseMenu.h"

void UGameHUD::NativeConstruct()
{
	Super::NativeConstruct();

	if (PauseButton)
	{
		PauseButton->OnClicked.AddDynamic(this, &UGameHUD::OnPauseClick);
	}

	PauseMenu = CreateWidget<UPauseMenu>(GetWorld(), PauseMenuWidgetClass);
	if (PauseMenu)
	{
		PauseMenu->SetGameHUD(this);
	}
	

	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(UGameplayStatics::GetPlayerController(this, 0));
	//FInputModeGameAndUI GameandUI;
	//UGameplayStatics::GetPlayerController(this, 0)->SetInputMode(GameandUI);
}

void UGameHUD::InitializedHUD(ARunnerGameModeBase* GameMode)
{
	if (GameMode)
	{
		CoinsCount->SetText(FText::AsNumber(0));
		LivesCount->SetText(FText::AsNumber(GameMode->MaxLives));

		GameMode->OnCoinsCountChanged.AddDynamic(this, &ThisClass::SetCoinsCount);
		GameMode->OnLivesCountChanged.AddDynamic(this, &ThisClass::SetLivesCount);
		
	}
}

void UGameHUD::SetCoinsCount(const int32 Coin)
{
	CoinsCount->SetText(FText::AsNumber(Coin));
}

void UGameHUD::SetLivesCount(const int32 Lives)
{
	LivesCount->SetText(FText::AsNumber(Lives));
}

void UGameHUD::OnPauseClick()
{
	if (bPauseButtonClick) return;

	bPauseButtonClick = true;
	

	if (IsValid(PauseMenuWidgetClass))
	{

		bPauseButtonClick = true;

		UGameplayStatics::SetGamePaused(this, true);
		PauseMenu == nullptr ? CreateWidget<UPauseMenu>(GetWorld(), PauseMenuWidgetClass) : PauseMenu;
		if (PauseMenu)
		{
			PauseMenu->AddToViewport();
			
		}
	}
	
}

