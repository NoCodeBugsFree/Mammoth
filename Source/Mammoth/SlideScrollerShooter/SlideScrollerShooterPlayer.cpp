// Fill out your copyright notice in the Description page of Project Settings.

#include "SlideScrollerShooterPlayer.h"

ASlideScrollerShooterPlayer::ASlideScrollerShooterPlayer()
{

}

void ASlideScrollerShooterPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	/** Fire  */
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASlideScrollerShooterPlayer::Fire);

	/** MoveUp  */
	PlayerInputComponent->BindAxis("MoveUp", this, &ASlideScrollerShooterPlayer::MoveUp);
}

void ASlideScrollerShooterPlayer::MoveUp(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}
