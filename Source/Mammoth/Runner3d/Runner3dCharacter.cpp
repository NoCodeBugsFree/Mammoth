// Fill out your copyright notice in the Description page of Project Settings.

#include "Runner3dCharacter.h"


// Sets default values
ARunner3dCharacter::ARunner3dCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	JumpMaxHoldTime = 0.6f;
}

// Called when the game starts or when spawned
void ARunner3dCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARunner3dCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddMovementInput(GetActorForwardVector());
}

// Called to bind functionality to input
void ARunner3dCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/** Jump  */
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ARunner3dCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ARunner3dCharacter::StopJumping);
}

