// Fill out your copyright notice in the Description page of Project Settings.

#include "GravityPlatformerPlayer.h"
#include "ConstructorHelpers.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GravityPlatformerObstacle.h"
#include "GravityPlatformerPickUp.h"
#include "Kismet/GameplayStatics.h"

AGravityPlatformerPlayer::AGravityPlatformerPlayer()
{
	/**  set the default static mesh */
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ConeTemplate(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (ConeTemplate.Succeeded())
	{
		ConeMesh->SetStaticMesh(ConeTemplate.Object);
	}

	CameraBoom->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));

	GetCapsuleComponent()->SetCapsuleHalfHeight(50.f);
	GetCapsuleComponent()->SetCapsuleRadius(50.f);

	/** sensitive air control  */
	GetCharacterMovement()->AirControl = 1.f;

	/** simulate ceiling movement as floor as well  */
	GetCharacterMovement()->bUseSeparateBrakingFriction = true;
	GetCharacterMovement()->BrakingFriction = 10.f;

	/** defaults  */
	bCanChangeGravity = true;
}

void AGravityPlatformerPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	/** find all pickups and update PickupsToCollect  */
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGravityPlatformerPickUp::StaticClass(), FoundActors);
	PickupsToCollect = FoundActors.Num();

	if (PickupsToCollect == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Place some pickups to the scene..."));
		LoseTheGame();
	}

	GetWorldTimerManager().SetTimer(LevelTimer, this, &AGravityPlatformerPlayer::LoseTheGame, LevelTime, false);
}

void AGravityPlatformerPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	/** movement  */
	PlayerInputComponent->BindAxis("MoveRight", this, &AGravityPlatformerPlayer::MoveY);

	/** gravity  */
	PlayerInputComponent->BindAction("ChangeGravity", IE_Pressed, this, &AGravityPlatformerPlayer::ChangeGravity);
}

void AGravityPlatformerPlayer::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if (Other && Other != this && !IsPendingKill())
	{
		bCanChangeGravity = true;
	}
}

void AGravityPlatformerPlayer::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	if (OtherActor && OtherActor != this && !OtherActor->IsPendingKill())
	{
		/** pickup add score  */
		if(AGravityPlatformerPickUp* PickUp = Cast<AGravityPlatformerPickUp>(OtherActor))
		{
			/** disable the pickup  */
			PickUp->SetActorHiddenInGame(true);
			PickUp->SetActorEnableCollision(false);
			PickUp->Destroy();

			/** update score and check for win the game  */
			Score++;
			if (Score == PickupsToCollect)
			{
				WinTheGame();
			}

			return;
		}

		/** obstacle - kill the player, restart the game  */
		if (AGravityPlatformerObstacle* Obstacle = Cast<AGravityPlatformerObstacle>(OtherActor))
		{
			LoseTheGame();
			return;
		}
	}
}

void AGravityPlatformerPlayer::ChangeGravity()
{
	/** to prevent change gravity until we hit the floor or ceiling  */
	if (bCanChangeGravity)
	{
		/** reverse gravity  */
		GetCharacterMovement()->GravityScale = -GetCharacterMovement()->GravityScale;

		/** I want to fly  */
		Jump();

		bCanChangeGravity = false;
	}
}
