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
		QuitButton->OnClicked.AddDynamic(this, &ThisClass::OnQuitClick);
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
	
	if (GameHUD)
	{
		GameHUD->bPauseButtonClick = false;
	}
	RemoveFromParent();
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

void UPauseMenu::OnQuitClick()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UKismetSystemLibrary::ExecuteConsoleCommand(World, TEXT("Quit"));
		//UKismetSystemLibrary::QuitGame(GetWorld(),UGameplayStatics::GetPlayerController(this,0),EQuitPreference::Quit,true);
	}
}


