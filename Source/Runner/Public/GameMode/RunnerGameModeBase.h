// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RunnerGameModeBase.generated.h"

/**
 * 
 */
class AFloorTile;
class UUserWidget;
class UGameHUD;
class UGameOver;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCoinsCountChanged, int32, CoinsCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLivesCountChanged, int32, LivesCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelReset);

UCLASS()
class RUNNER_API ARunnerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable)
	void CreateInitialFloorTiles();

	UFUNCTION(BlueprintCallable)

	AFloorTile* AddFloorTile(const bool bSpawnItems);

	void AddCoin();

	void PlayerDied();

	void RemoveTile(AFloorTile* ToRemoveTile);

	void GameOver();

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Delegates")
	FOnLevelReset OnLevelReset;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Delegates")
	FOnCoinsCountChanged OnCoinsCountChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Delegates")
	FOnLivesCountChanged OnLivesCountChanged;

	UPROPERTY(EditAnywhere, Category = "Config")
	TSubclassOf<UUserWidget> GameHUDClass;

	UPROPERTY(EditAnywhere, Category = "Config")
	TSubclassOf<AFloorTile> FloorTileClass;

	UPROPERTY(EditAnywhere, Category = "Config")
	TSubclassOf<UGameOver> GameOverClass;

	UPROPERTY()
	AFloorTile* Tile;

	UPROPERTY(VisibleInstanceOnly, Category = "Config")
	UGameHUD* GameHUD;

	UPROPERTY(EditAnywhere, Category = "Config")
	int32 NumInitialFloorTiles = 10;

	UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
	FTransform NextSpawnPoint;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly,Category = "Runtime")
	TArray<float> LaneSwitchValues;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Runtime")
	TArray<AFloorTile*> FloorTiles;

	int32 TotalCoins = 0;

	int32 NumberOfLives = 0;

	UPROPERTY(EditAnywhere)
	int32 MaxLives = 3;
};
