// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacle.generated.h"

UCLASS()
class RUNNER_API AObstacle : public AActor
{
	GENERATED_BODY()
	
public:	
	AObstacle();
	

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* StaticMesh;


protected:
	UFUNCTION()
	void OnObstacleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:	
	

};
