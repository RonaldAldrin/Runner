// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG/MainMenu.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();
	if (StartButton && QuitButton)
	{
		StartButton->OnClicked.AddDynamic(this, &ThisClass::OnStartClick);
		QuitButton->OnClicked.AddDynamic(this, &ThisClass::OnQuitClick);
	}

}

void UMainMenu::OnStartClick()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UGameplayStatics::OpenLevel(World, FName("MainLevelMap"));
	}
	
}

void UMainMenu::OnQuitClick()
{


	UWorld* World = GetWorld();
	if (World)
	{
		UKismetSystemLibrary::ExecuteConsoleCommand(World, TEXT("Quit"));
		//UKismetSystemLibrary::QuitGame(GetWorld(),UGameplayStatics::GetPlayerController(this,0),EQuitPreference::Quit,true);
	}
}
