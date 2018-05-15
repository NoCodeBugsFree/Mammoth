// Fill out your copyright notice in the Description page of Project Settings.

#include "PlatformSwitcherPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PlatformSwitcherKillBlock.h"

APlatformSwitcherPlayer::APlatformSwitcherPlayer()
{
	/** camera boom  */
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 1800.f;
	CameraBoom->SetRelativeRotation(FRotator(-20.f, 0.f, 0.f));
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritRoll = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bDoCollisionTest = false;

	/* camera component  */
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraBoom);
	CameraComponent->SetRelativeRotation(FRotator(10.f, 0.f, 0.f));

	/** class defaults  */
	bCanTeleport = true;
}

void APlatformSwitcherPlayer::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor && OtherActor != this && OtherActor->IsA(APlatformSwitcherKillBlock::StaticClass()))
	{
		Lose();
	}
}

void APlatformSwitcherPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/** teleport  */
	PlayerInputComponent->BindAction("FirstLine", IE_Pressed, this, &APlatformSwitcherPlayer::TeleportToFirstLine);
	PlayerInputComponent->BindAction("SecondLine", IE_Pressed, this, &APlatformSwitcherPlayer::TeleportToSecondLine);
}

void APlatformSwitcherPlayer::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();
}

void APlatformSwitcherPlayer::TeleportToFirstLine()
{
	if (bCanTeleport)
	{
		bCanTeleport = false;

		PlayTeleportFX();
		FVector CurrentLocation = GetActorLocation();
		FVector DesiredLocation = CurrentLocation;
		DesiredLocation.X = StartLocation.X;
		SetActorLocation(DesiredLocation);
		
		ResetTeleport();
	}
}

void APlatformSwitcherPlayer::TeleportToSecondLine()
{
	if (bCanTeleport)
	{
		bCanTeleport = false;

		PlayTeleportFX();
		FVector CurrentLocation = GetActorLocation();
		FVector DesiredLocation = CurrentLocation;
		DesiredLocation.X = SecondLine.X;
		SetActorLocation(DesiredLocation);
		ResetTeleport();
	}
}

void APlatformSwitcherPlayer::ResetTeleport()
{
	FTimerDelegate ResetTeleportDelegate;
	ResetTeleportDelegate.BindLambda([&] { bCanTeleport = true;  });

	FTimerHandle ResetTeleportTimer;
	GetWorldTimerManager().SetTimer(ResetTeleportTimer, ResetTeleportDelegate, 1.f, false);
}

