// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Coin.generated.h"

UCLASS()
class RUNNER_API ACoin : public AActor
{
	GENERATED_BODY()
	
public:	

	ACoin();

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);



	UPROPERTY(VisibleAnywhere)
	class USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* CoinMesh;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* SphereCollider;

	UPROPERTY(VisibleAnywhere)
	class URotatingMovementComponent* RotatingMovement;

	UPROPERTY(EditAnywhere)
	class USoundBase* OverlapSound;

protected:
	virtual void BeginPlay() override;

public:	


};
