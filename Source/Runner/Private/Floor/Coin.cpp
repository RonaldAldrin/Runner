// Fill out your copyright notice in the Description page of Project Settings.


#include "Floor/Coin.h"
#include "Components/SphereComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Character/RunCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

ACoin::ACoin()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComponent);

	SphereCollider = CreateDefaultSubobject <USphereComponent>(TEXT("SphereCollider"));
	SphereCollider->SetupAttachment(GetRootComponent());
	SphereCollider->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	CoinMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CoinMesh"));
	CoinMesh->SetupAttachment(SphereCollider);
	CoinMesh->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovement"));
	RotatingMovement->RotationRate = FRotator(0.f, 180.f, 0.f);

}

void ACoin::BeginPlay()
{
	Super::BeginPlay();

	SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &ACoin::OnSphereOverlap);
}

void ACoin::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(ARunCharacter* Character = Cast<ARunCharacter>(OtherActor))
	{
		if (OverlapSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, OverlapSound,GetActorLocation());
		}

		Character->AddCoin();

		Destroy();
	}
}

