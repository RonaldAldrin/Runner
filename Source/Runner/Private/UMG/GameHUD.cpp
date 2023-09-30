// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG/GameHUD.h"
#include "Components/TextBlock.h"
#include "GameMode/RunnerGameModeBase.h"

void UGameHUD::InitializedHUD(ARunnerGameModeBase* GameMode)
{
	if (GameMode)
	{
		CoinsCount->SetText(FText::AsNumber(0));

		GameMode->OnCoinsCountChanged.AddDynamic(this, &ThisClass::SetCoinsCount);
	}
}

void UGameHUD::SetCoinsCount(const int32 Coin)
{
	CoinsCount->SetText(FText::AsNumber(Coin));
}
