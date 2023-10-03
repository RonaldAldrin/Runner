// Fill out your copyright notice in the Description page of Project Settings.

//game
#include "Character/RunCharacter.h"
#include "GameMode/RunnerGameModeBase.h"


// engine
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerStart.h"


ARunCharacter::ARunCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 350.f;
	SpringArm->SocketOffset = FVector(0.f, 0.f, 100.f);
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	SwitchTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("SwitchTimeline"));

}


void ARunCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveLeftAction, ETriggerEvent::Started, this, &ThisClass::MoveLeft);
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Started, this, &ThisClass::MoveRight);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(MoveDownAction, ETriggerEvent::Started, this, &ThisClass::MoveDown);
	}

}

void ARunCharacter::BeginPlay()
{
	Super::BeginPlay();

	RunGameMode = Cast<ARunnerGameModeBase>(UGameplayStatics::GetGameMode(this));
	check(RunGameMode);

	RunGameMode->OnLevelReset.AddDynamic(this, &ThisClass::ResetLevel);

	PlayerStart = Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass()));

	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		UEnhancedInputLocalPlayerSubsystem* EnhancedInput = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		if (EnhancedInput && InputMapping)
		{
			EnhancedInput->AddMappingContext(InputMapping, 0);
		}
	}


	MovementValue.BindUFunction(this, FName("ChangeLaneUpdate"));
	TimelineFinishedEvent.BindUFunction(this, FName("ChangeLaneFinished"));

	if (MovementCurve && SwitchTimeline)
	{
		SwitchTimeline->AddInterpFloat(MovementCurve, MovementValue);
		SwitchTimeline->SetTimelineFinishedFunc(TimelineFinishedEvent);
	}

}

void ARunCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunForward();
}

void ARunCharacter::ChangeLaneUpdate(const float Value)
{
	FVector Location = GetCapsuleComponent()->GetComponentLocation();
	Location.Y = FMath::Lerp(RunGameMode->LaneSwitchValues[CurrentLane], RunGameMode->LaneSwitchValues[NextLane], Value);
	SetActorLocation(Location);
	
	
}

void ARunCharacter::ChangeLaneFinished()
{
	CurrentLane = NextLane;
}


void ARunCharacter::ChangeLane()
{
	if (SwitchTimeline)
	{
		SwitchTimeline->PlayFromStart();
	}

}


void ARunCharacter::RunForward()
{
	if (bIsDead) return;

	// find out which way is forward
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.f;
	ControlRot.Roll = 0.f;

	AddMovementInput(ControlRot.Vector());
}


void ARunCharacter::MoveLeft()
{
	//FVector2D MoveRight = Value.Get<FVector2D>();
	//if (Controller != nullptr && MoveRight != FVector2D(0.f))
	//{
	//	// find out which way is forward
	//	FRotator Rotation = Controller->GetControlRotation();
	//	FRotator YawRotation{ 0.f, Rotation.Yaw, 0.f };

	//	// get right vector 
	//	FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	//	AddMovementInput(RightDirection, MoveRight.X);
	//}

	/*MovementValue.BindDynamic(this, &ThisClass::ChangeLaneUpdate);
	TimelineFinishedEvent.BindDynamic(this, &ThisClass::ChangeLaneFinished);

	if (MovementCurve && SwitchTimeline)
	{
		SwitchTimeline->AddInterpFloat(MovementCurve, MovementValue);
		SwitchTimeline->SetTimelineFinishedFunc(TimelineFinishedEvent);
	}*/

	NextLane = FMath::Clamp(CurrentLane - 1, 0, 2);
	ChangeLane();
	
}

void ARunCharacter::MoveRight()
{
	NextLane = FMath::Clamp(CurrentLane + 1, 0, 2);
	ChangeLane();

}

void ARunCharacter::MoveDown()
{
	static FVector Impulse = FVector(0.0f, 0.0f, MoveDownImpulse);
	GetCharacterMovement()->AddImpulse(Impulse, true);

	//UE_LOG(LogTemp, Warning, TEXT("S button is pressed"));
}

void ARunCharacter::ResetLevel()
{
	bIsDead = false;
	EnableInput(nullptr);
	GetMesh()->SetVisibility(true);

	if (PlayerStart)
	{
		SetActorLocation(PlayerStart->GetActorLocation());
		SetActorRotation(PlayerStart->GetActorRotation());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid PlayerStart"));
	}
}

void ARunCharacter::Die()
{
	if (bIsDead) return;

	FVector Location = GetMesh()->GetComponentLocation();
	if (DeathParticle)
	{
		bIsDead = true;
		DisableInput(nullptr);

		Location.Z += 80.f;
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathParticle, Location, FRotator(0.f, 0.f, 0.f), FVector(3.f), false);
	}
	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, Location, FRotator(0.f, 0.f, 0.f));
	}

	GetMesh()->SetVisibility(false);
	GetWorldTimerManager().SetTimer(DeathTimer, this, &ThisClass::OnDeathFinished, 2.5f);

	//Destroy();

	
	
}

void ARunCharacter::OnDeathFinished()
{
	
	if (DeathTimer.IsValid())
	{
		GetWorldTimerManager().ClearTimer(DeathTimer);
	}

	RunGameMode->PlayerDied();



	//UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), TEXT("RestartLevel")); //UGameplayStatics::OpenLevel(this, FName("TestMap")); same
	
	
}

void ARunCharacter::AddCoin()
{
	RunGameMode->AddCoin();
}



