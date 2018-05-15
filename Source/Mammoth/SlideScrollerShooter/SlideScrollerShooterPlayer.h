// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpaceShooter/SpaceShooterPlayer.h"
#include "SlideScrollerShooterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class MAMMOTH_API ASlideScrollerShooterPlayer : public ASpaceShooterPlayer
{
	GENERATED_BODY()
	
public:

	ASlideScrollerShooterPlayer();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
private:

	void MoveUp(float Value);
	
};
