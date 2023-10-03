// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/RunnerGameModeBase.h"
#include "Floor/FloorTile.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UMG/GameHUD.h"
#include "Components/TextBlock.h"
#include "UMG/GameOver.h"
#include "Character/RunCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void ARunnerGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetPlayerController(this, 0)->bShowMouseCursor = true;

	GameHUD = Cast<UGameHUD>(CreateWidget(GetWorld(), GameHUDClass));
	check(GameHUD);

	GameHUD->InitializedHUD(this);
	GameHUD->AddToViewport();

	NumberOfLives = MaxLives;

	CreateInitialFloorTiles();
}

void ARunnerGameModeBase::CreateInitialFloorTiles()
{
	Tile = AddFloorTile(false);
	if (Tile)
	{
		LaneSwitchValues.Add(Tile->GetLeftLane()->GetComponentLocation().Y);
		LaneSwitchValues.Add(Tile->GetCenterLane()->GetComponentLocation().Y);
		LaneSwitchValues.Add(Tile->GetRightLane()->GetComponentLocation().Y);
		
	}

	/*for (float Val : LaneSwitchValues)
	{
		UE_LOG(LogTemp, Warning, TEXT("Lane Value: %f"), Val);
	}*/

	AddFloorTile(false);
	AddFloorTile(false);

	for (int i = 0; i < NumInitialFloorTiles; i++)
	{
		AddFloorTile(true);
	}
}

AFloorTile* ARunnerGameModeBase::AddFloorTile(const bool bSpawnItems)
{
	UWorld* World = GetWorld();
	if (World)
	{
		AFloorTile* newTile = World->SpawnActor<AFloorTile>(FloorTileClass,NextSpawnPoint);
		if (newTile)
		{
			FloorTiles.Add(newTile);

			if (bSpawnItems)
			{
				newTile->SpawnItems();
			}

			NextSpawnPoint = newTile->GetAttachPointTransform();
		}
		return newTile;
	}
	return nullptr;
}

void ARunnerGameModeBase::AddCoin()
{
	TotalCoins += 1;

	//GameHUD->SetCoinsCount(TotalCoins);
	OnCoinsCountChanged.Broadcast(TotalCoins); 

	
	
	APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0);
	if (Controller)
	{
		ARunCharacter* Character = Cast<ARunCharacter>(Controller->GetPawn());
		if (Character)
		{
			
			if (TotalCoins > 30)Character->GetCharacterMovement()->MaxWalkSpeed = 2000.f;
			if (TotalCoins > 50)Character->GetCharacterMovement()->MaxWalkSpeed = 3000.f;
			if (TotalCoins > 100)Character->GetCharacterMovement()->MaxWalkSpeed = 4500.f;
			
		}
		
			
	}
		
	
		
	
	/*FString Coin = FString::Printf(TEXT("%d"), TotalCoins);
	GameHUD->CoinsCount->SetText(FText::FromString(Coin));*/

	//UE_LOG(LogTemp, Warning, TEXT("TotalCoin : %d"), TotalCoins);
}

void ARunnerGameModeBase::PlayerDied()
{
	NumberOfLives = FMath::Clamp(NumberOfLives - 1, 0, MaxLives);

	//GameHUD->SetLivesCount(NumberOfLives);
	OnLivesCountChanged.Broadcast(NumberOfLives); 
	if (NumberOfLives > 0)
	{
		// Iterate all FloorTiles and call DestroyFloorTile
		int32 j = FloorTiles.Num() -1;
		AFloorTile* Tiles = FloorTiles[j];
		for (auto oldTile : FloorTiles )
		{
			if (oldTile)
			{
				oldTile->DestroyFloorTile();
			}
			
				
		}

		// Empty our array
		FloorTiles.Empty();

		// NextSpawnPoint to initial value
		NextSpawnPoint = FTransform{};

		// create out initial floor tiles
		CreateInitialFloorTiles();

		// Broadcast level reset event
		OnLevelReset.Broadcast();
	}
	else
	{
		GameOver();
	}
}

void ARunnerGameModeBase::RemoveTile(AFloorTile* ToRemoveTile)
{
	FloorTiles.Remove(ToRemoveTile);
}

void ARunnerGameModeBase::GameOver()
{
	if (IsValid(GameOverClass))
	{
		UGameOver* Widget = CreateWidget<UGameOver>(GetWorld(), GameOverClass);
		if (Widget)
		{
			Widget->AddToViewport();
			GameHUD->bPauseButtonClick = true;
		}
	}
}
