// Fill out your copyright notice in the Description page of Project Settings.


#include "Floor/Obstacle.h"
#include "Character/RunCharacter.h"

AObstacle::AObstacle()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComponent);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(GetRootComponent());

	StaticMesh->OnComponentHit.AddDynamic(this, &AObstacle::OnObstacleHit);
}

void AObstacle::OnObstacleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ARunCharacter* HitCharacter = Cast<ARunCharacter>(OtherActor);
	if (HitCharacter)
	{
		HitCharacter->Die();
	}
}



