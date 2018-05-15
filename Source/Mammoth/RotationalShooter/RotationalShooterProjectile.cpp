// Fill out your copyright notice in the Description page of Project Settings.

#include "RotationalShooterProjectile.h"
#include "ConstructorHelpers.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "RotationalShooterPlayer.h"
#include "RotationalShooterEnemy.h"

// Sets default values
ARotationalShooterProjectile::ARotationalShooterProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	/** sphere collision  */
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SetRootComponent(SphereCollision);
	SphereCollision->SetSphereRadius(25.f);
	SphereCollision->SetCollisionProfileName("OverlapAll");

	/** mesh  */
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));

	/**  set the default static mesh */
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Sphere(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (Sphere.Succeeded())
	{
		ProjectileMesh->SetStaticMesh(Sphere.Object);
	}

	/** projectile movement component  */
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->bInitialVelocityInLocalSpace = true;
	ProjectileMovementComponent->InitialSpeed = 1500.f;
	ProjectileMovementComponent->MaxSpeed = 1500.f;
	ProjectileMovementComponent->bConstrainToPlane = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;

	InitialLifeSpan = 3.f;

}

void ARotationalShooterProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	if (OtherActor && !OtherActor->IsPendingKill() && OtherActor != this && !IsPendingKill())
	{
		/** if we hit an enemy  */
		if (ARotationalShooterEnemy* Enemy = Cast<ARotationalShooterEnemy>(OtherActor))
		{
			/**  kill the enemy  */
			Enemy->SetActorEnableCollision(false);
			Enemy->SetActorHiddenInGame(true);
			Enemy->Destroy();

			/** update player's score  */
			if(ARotationalShooterPlayer* Owner = Cast<ARotationalShooterPlayer>(GetOwner()))
			{
				Owner->Score++;
			}

			/** disable self  */
			SetActorEnableCollision(false);
			SetActorHiddenInGame(false);
			ProjectileMovementComponent->StopMovementImmediately();
			ProjectileMovementComponent->SetComponentTickEnabled(false);

			/** pending destroy  */
			Destroy();
		}
	}
}
