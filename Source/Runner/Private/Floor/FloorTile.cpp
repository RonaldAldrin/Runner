// Fill out your copyright notice in the Description page of Project Settings.


#include "Floor/FloorTile.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameMode/RunnerGameModeBase.h"
#include "Character/RunCharacter.h"
#include "Floor/Obstacle.h"
#include "Floor/Coin.h"

AFloorTile::AFloorTile()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorMesh"));
	FloorMesh->SetupAttachment(SceneRoot);

	AttachPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("AttachPoint"));
	AttachPoint->SetupAttachment(SceneRoot);

	CenterLane = CreateDefaultSubobject<UArrowComponent>(TEXT("CenterLane"));
	CenterLane->SetupAttachment(SceneRoot);

	LeftLane = CreateDefaultSubobject<UArrowComponent>(TEXT("LeftLane"));
	LeftLane->SetupAttachment(SceneRoot);

	RightLane = CreateDefaultSubobject<UArrowComponent>(TEXT("RightLane"));
	RightLane->SetupAttachment(SceneRoot);

	FloorTriggerBox = CreateDefaultSubobject <UBoxComponent>(TEXT("FloorTriggerBox"));
	FloorTriggerBox->SetupAttachment(SceneRoot);
	FloorTriggerBox->SetBoxExtent(FVector(32.f,500.f,200.f));
	FloorTriggerBox->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	
}

void AFloorTile::BeginPlay()
{
	Super::BeginPlay();

	RunGameMode = Cast<ARunnerGameModeBase>(UGameplayStatics::GetGameMode(this));
	check(RunGameMode);

	FloorTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBoxOverlap);
	GetWorldTimerManager().SetTimer(CoinObstacleTimer, this, &ThisClass::DestroyCoinObstacle, 12.f, false);
}


void AFloorTile::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARunCharacter* Character = Cast<ARunCharacter>(OtherActor);
	if (Character)
	{
		RunGameMode->AddFloorTile(true);
		GetWorldTimerManager().SetTimer(DestroyTimerHandle,this,&ThisClass::DestroyFloorTile,2.f,false);
		
		
	}
}


void AFloorTile::DestroyFloorTile()
{
	
	if (DestroyTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
	}

	this->Destroy();

}

void AFloorTile::DestroyCoinObstacle()
{
	if (DestroyTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(CoinObstacleTimer);
	}
	if (Coin)
	{
		AActor* CoinToDestroy = UGameplayStatics::GetActorOfClass(this, CoinClass);
		if (CoinToDestroy)
		{
			CoinToDestroy->Destroy();
		}
	}
	if (Obstacle)
	{
		AActor* SmallObstacle = UGameplayStatics::GetActorOfClass(this, SmallObstacleClass);
		AActor* BigObstacle = UGameplayStatics::GetActorOfClass(this, BigObstacleClass);

		if (SmallObstacle || BigObstacle)
		{
			SmallObstacle->Destroy();
			BigObstacle->Destroy();
		}
	}
}


const FTransform& AFloorTile::GetAttachPointTransform() const
{
	return AttachPoint->GetComponentTransform();
}

void AFloorTile::SpawnItems()
{
	if (IsValid(SmallObstacleClass) && IsValid(BigObstacleClass) && IsValid(CoinClass))
	{
		SpawnLaneItems(CenterLane);
		SpawnLaneItems(LeftLane);
		SpawnLaneItems(RightLane);
	}
	

}

void AFloorTile::SpawnLaneItems(UArrowComponent* Lane)
{
	const float RandVal = FMath::FRandRange(0.f, 1.f);

	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	const FTransform& SpawnLocation = Lane->GetComponentTransform();

	if (UKismetMathLibrary::InRange_FloatFloat(RandVal, SpawnPercent1, SpawnPercent2, true, true))// same as RandVal >= 0.5f && RandVal <= 1.f
	{
		Obstacle = GetWorld()->SpawnActor<AObstacle>(SmallObstacleClass, SpawnLocation, SpawnParams);
		
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(RandVal, SpawnPercent2, SpawnPercent3, true, true))
	{
		Obstacle = GetWorld()->SpawnActor<AObstacle>(BigObstacleClass, SpawnLocation, SpawnParams);
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(RandVal, SpawnPercent3, 1.f, true, true))
	{
		Coin = GetWorld()->SpawnActor<ACoin>(CoinClass, SpawnLocation, SpawnParams);
	}
}

