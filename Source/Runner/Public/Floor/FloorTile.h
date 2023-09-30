// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloorTile.generated.h"

class USceneComponent;
class UArrowComponent;
class UStaticMeshComponent;
class UBoxComponent;
class ARunnerGameModeBase;
class AObstacle;
class ACoin;

UCLASS()
class RUNNER_API AFloorTile : public AActor
{
	GENERATED_BODY()
	
public:	
	AFloorTile();
	

	UFUNCTION(BlueprintCallable)
	void SpawnItems();

	UFUNCTION()
	void DestroyFloorTile();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void SpawnLaneItems(UArrowComponent* Lane, int32& NumBig);


	FTimerHandle CoinObstacleTimer;
	FTimerHandle DestroyTimerHandle;


private:

	UPROPERTY()
	AObstacle* Obstacle;

	UPROPERTY(EditAnywhere, Category = "Config")
	TSubclassOf<AObstacle> SmallObstacleClass;

	UPROPERTY(EditAnywhere, Category = "Config")
	TSubclassOf<AObstacle> BigObstacleClass;

	UPROPERTY(EditAnywhere, Category = "Config")
	TSubclassOf<ACoin> CoinClass;

	UPROPERTY()
	ACoin* Coin;

	UPROPERTY(VisibleInstanceOnly)
	ARunnerGameModeBase* RunGameMode;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* FloorMesh;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UArrowComponent* AttachPoint;

	UPROPERTY(VisibleAnywhere)
	UArrowComponent* CenterLane;

	UPROPERTY(VisibleAnywhere)
	UArrowComponent* LeftLane;

	UPROPERTY(VisibleAnywhere)
	UArrowComponent* RightLane;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* FloorTriggerBox;

	UPROPERTY(EditAnywhere)
	float SpawnPercent1 = 0.1f;

	UPROPERTY(EditAnywhere)
	float SpawnPercent2 = 0.3f;

	UPROPERTY(EditAnywhere)
	float SpawnPercent3 = 0.5f;

	TArray<AActor*> ChildActors;

public:	
	
	const FTransform& GetAttachPointTransform() const;

	FORCEINLINE UArrowComponent* GetLeftLane() const { return LeftLane; }
	FORCEINLINE UArrowComponent* GetRightLane() const { return RightLane; }
	FORCEINLINE UArrowComponent* GetCenterLane() const { return CenterLane; }
	
		
	

};
