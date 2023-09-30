// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "RunCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UCurveFloat;
class ARunnerGameModeBase;
class UParticleSystem;
class USoundCue;
class APlayerStart;

UCLASS()
class RUNNER_API ARunCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ARunCharacter();
	virtual void Tick(float DeltaTime) override;
	void RunForward();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void ChangeLane();

	UFUNCTION(BlueprintCallable, Category = "Lane")
	void ChangeLaneUpdate(float Value);

	UFUNCTION(BlueprintCallable,Category = "Lane")
	void ChangeLaneFinished();

	
	void Die();

	UFUNCTION()
	void OnDeathFinished();

	void AddCoin();

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	int32 CurrentLane =1;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	int32 NextLane = 0;

	FTimerHandle DeathTimer;

	bool bIsDead;

protected:
	virtual void BeginPlay() override;

	/** Called for movement input */
	void MoveLeft();

	void MoveRight();

	void MoveDown();

	UFUNCTION()
	void ResetLevel();

	APlayerStart* PlayerStart;

private:

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly,Category = InputMapping)
	UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, Category = InputMapping)
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = InputMapping)
	UInputAction* MoveDownAction;

	UPROPERTY(EditDefaultsOnly, Category = InputMapping)
	UInputAction* MoveLeftAction;

	UPROPERTY(EditDefaultsOnly, Category = InputMapping)
	UInputAction* MoveRightAction;



	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* SwitchTimeline;

	UPROPERTY(EditAnywhere, Category ="Curve")
	UCurveFloat* MovementCurve;

	FOnTimelineFloat MovementValue;
	FOnTimelineEvent TimelineFinishedEvent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	ARunnerGameModeBase* RunGameMode;

	UPROPERTY(EditAnywhere, Category = "Config")
	UParticleSystem* DeathParticle;

	UPROPERTY(EditAnywhere, Category = "Config")
	USoundCue* DeathSound;

	UPROPERTY(EditAnywhere,Category = "Config")
	float MoveDownImpulse = -1000.f;

public:	

	FORCEINLINE ARunnerGameModeBase* GetRunGameMode() const { return RunGameMode; }
	

};
