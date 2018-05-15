// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceShooterProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PaperSpriteComponent.h"
#include "SpaceShooterEnemy.h"
#include "SpaceShooterPlayer.h"

ASpaceShooterProjectile::ASpaceShooterProjectile()
{
	GetRenderComponent()->SetMobility(EComponentMobility::Movable);

	/* projectile movement component  */
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->MaxSpeed = 2500.f;
	ProjectileMovementComponent->InitialSpeed = 2500.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;

	InitialLifeSpan = 3.f;
}

void ASpaceShooterProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor && OtherActor != this && !IsPendingKill() && !OtherActor->IsPendingKill())
	{
		if (ASpaceShooterEnemy* Enemy = Cast<ASpaceShooterEnemy>(OtherActor))
		{
			PlayImpactFX();
			
			if (ASpaceShooterPlayer* Player = Cast<ASpaceShooterPlayer>(GetOwner()))
			{
				Player->AddScore();
			}

			Enemy->Destroy();

			/** disable projectile  */
			SetActorHiddenInGame(true);
			SetActorEnableCollision(false);
			ProjectileMovementComponent->StopMovementImmediately();
		}
	}
}
