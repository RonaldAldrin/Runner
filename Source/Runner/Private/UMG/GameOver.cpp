// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG/GameOver.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UMG/GameHUD.h"

void UGameOver::NativeConstruct()
{
	Super::NativeConstruct();

	if (MainMenuButton && RestartButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &ThisClass::OnMainMenuClick);
		RestartButton->OnClicked.AddDynamic(this, &ThisClass::OnRestartClick);
	}

}

void UGameOver::OnMainMenuClick()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UGameplayStatics::OpenLevel(World, FName("MainMenuMap"));
	}
}

void UGameOver::OnRestartClick()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UKismetSystemLibrary::ExecuteConsoleCommand(World, TEXT("RestartLevel"));
	}
}
