// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RunAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class RUNNER_API URunAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

public:

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Movement")
	bool bIsInAir;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Movement")
	float Speed;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Movement")
	class APawn* Pawn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Movement")
	class UPawnMovementComponent* MovementComponent;

protected:

};
