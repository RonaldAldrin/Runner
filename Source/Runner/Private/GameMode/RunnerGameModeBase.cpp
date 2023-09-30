// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/RunnerGameModeBase.h"
#include "Floor/FloorTile.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UMG/GameHUD.h"
#include "Components/TextBlock.h"

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
	AFloorTile* Tile = AddFloorTile(false);
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
		AFloorTile* Tile = World->SpawnActor<AFloorTile>(FloorTileClass,NextSpawnPoint);
		if (Tile)
		{
			FloorTiles.Add(Tile);

			if (bSpawnItems)
			{
				Tile->SpawnItems();
			}

			NextSpawnPoint = Tile->GetAttachPointTransform();
		}
		return Tile;
	}
	return nullptr;
}

void ARunnerGameModeBase::AddCoin()
{
	TotalCoins += 1;

	GameHUD->SetCoinsCount(TotalCoins);
	OnCoinsCountChanged.Broadcast(TotalCoins); 


	/*FString Coin = FString::Printf(TEXT("%d"), TotalCoins);
	GameHUD->CoinsCount->SetText(FText::FromString(Coin));*/

	//UE_LOG(LogTemp, Warning, TEXT("TotalCoin : %d"), TotalCoins);
}

void ARunnerGameModeBase::PlayerDied()
{
	NumberOfLives--;

	GameHUD->SetLivesCount(NumberOfLives);
	OnLivesCountChanged.Broadcast(NumberOfLives); 
	if (NumberOfLives > 0)
	{
		// Iterate all FloorTiles and call DestroyFloorTile

		for (AFloorTile* Tile : FloorTiles)
		{
			Tile->DestroyFloorTile();
		}

	
		// Empty our array
		FloorTiles.Empty();

		// NextSpawnPoint to initial value
		NextSpawnPoint = FTransform();

		// create out initial floor tiles
		CreateInitialFloorTiles();

		// Broadcast level reset event
		OnLevelReset.Broadcast();
	}
	else
	{
		// GameOver();
	}
}

void ARunnerGameModeBase::RemoveTile(AFloorTile* Tile)
{
	FloorTiles.Remove(Tile);
}
