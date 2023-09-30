// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RunAnimInstance.h"

#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void URunAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (Pawn == nullptr)
	{
		Pawn = Cast<APawn>(TryGetPawnOwner());

		if (IsValid(Pawn))
		{
			MovementComponent = Pawn->GetMovementComponent();
		}
	}


}

void URunAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Pawn && MovementComponent)
	{
		bIsInAir = MovementComponent->IsFalling();
		Speed = Pawn->GetVelocity().Size();
	}


}
