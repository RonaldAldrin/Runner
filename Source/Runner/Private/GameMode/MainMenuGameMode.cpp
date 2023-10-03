// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/MainMenuGameMode.h"
#include "UMG/MainMenu.h"
#include "Kismet/GameplayStatics.h"

void AMainMenuGameMode::BeginPlay()
{
	if (IsValid(MainMenuClass))
	{
		UMainMenu* MainMenu = CreateWidget<UMainMenu>(GetWorld(), MainMenuClass);
		if (MainMenu)
		{
			MainMenu->AddToViewport();
		}

	}
	UGameplayStatics::GetPlayerController(this, 0)->SetShowMouseCursor(true);
}
