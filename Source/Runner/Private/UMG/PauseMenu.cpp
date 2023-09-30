// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG/PauseMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UMG/GameHUD.h"

void UPauseMenu::NativeConstruct()
{
	if (ContinueButton && RestartButton)
	{
		ContinueButton->OnClicked.AddDynamic(this, &ThisClass::OnContinueClick);
		RestartButton->OnClicked.AddDynamic(this, &ThisClass::OnRestartClick);
	}
	if (GameHUD)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameHud Is Valid in PauseMenuWidget"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GameHud Is InValid in PauseMenuWidget"));
	}

}

void UPauseMenu::OnContinueClick()
{
	UGameplayStatics::SetGamePaused(this, false);
	//SetVisibility(ESlateVisibility::Collapsed);
	RemoveFromParent();
	if (GameHUD)
	{
		GameHUD->bPauseButtonClick = false;
	}
}

void UPauseMenu::OnRestartClick()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UKismetSystemLibrary::ExecuteConsoleCommand(World, TEXT("RestartLevel"));
		//UGameplayStatics::OpenLevel(this, FName("TestMap"));
	}
	
}


