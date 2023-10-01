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
	
}


void AFloorTile::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARunCharacter* Character = Cast<ARunCharacter>(OtherActor);
	if (Character)
	{
		RunGameMode->AddFloorTile(true);


		GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &ThisClass::DestroyFloorTile, 2.f, false);
	}
}


void AFloorTile::DestroyFloorTile()
{
	
	if (DestroyTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
	}



	for (int32 i = 0; i < ChildActors.Num() ; i++)
	{
		AActor* Child = ChildActors[i];
		if (Child)
		{
			Child->Destroy();
		}
	}

	ChildActors.Empty();

	//RunGameMode->RemoveTile(this);

	Destroy();

}

const FTransform& AFloorTile::GetAttachPointTransform() const
{
	return AttachPoint->GetComponentTransform();
}

void AFloorTile::SpawnItems()
{
	if (IsValid(SmallObstacleClass) && IsValid(BigObstacleClass) && IsValid(CoinClass))
	{
		int32 NumBig = 0;
		SpawnLaneItems(CenterLane, NumBig);
		SpawnLaneItems(LeftLane, NumBig);
		SpawnLaneItems(RightLane, NumBig);
	}
	

}

void AFloorTile::SpawnLaneItems(UArrowComponent* Lane, int32& NumBig)
{
	const float RandVal = FMath::FRandRange(0.f, 1.f);

	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	const FTransform& SpawnLocation = Lane->GetComponentTransform();

	if (UKismetMathLibrary::InRange_FloatFloat(RandVal, SpawnPercent1, SpawnPercent2, true, true))// same as RandVal >= 0.5f && RandVal <= 1.f
	{
		Obstacle = GetWorld()->SpawnActor<AObstacle>(SmallObstacleClass, SpawnLocation, SpawnParams);
		ChildActors.Add(Obstacle);
		
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(RandVal, SpawnPercent2, SpawnPercent3, true, true))
	{
		NumBig++;
		UE_LOG(LogTemp, Warning, TEXT("NumBig: %d"), NumBig)
		if (NumBig <= 2)
		{
			Obstacle = GetWorld()->SpawnActor<AObstacle>(BigObstacleClass, SpawnLocation, SpawnParams);
			ChildActors.Add(Obstacle);
		}
		else if(NumBig == 3)
		{
			Obstacle = GetWorld()->SpawnActor<AObstacle>(SmallObstacleClass, SpawnLocation, SpawnParams);
			ChildActors.Add(Obstacle);
		}
		
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(RandVal, SpawnPercent3, 1.f, true, true))
	{
		Coin = GetWorld()->SpawnActor<ACoin>(CoinClass, SpawnLocation, SpawnParams);
		ChildActors.Add(Coin);
	}
}

