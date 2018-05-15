// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceShooterEnemy.h"
#include "ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "SpaceShooterPlayer.h"
#include "SpaceShooterProjectile.h"

ASpaceShooterEnemy::ASpaceShooterEnemy()
{
	/** mesh  */
	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMesh"));
	SetRootComponent(EnemyMesh);
	EnemyMesh->SetCollisionProfileName("Projectile");
	EnemyMesh->BodyInstance.bNotifyRigidBodyCollision = true;

	/**  set the default static mesh */
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Sphere(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (Sphere.Succeeded())
	{
		EnemyMesh->SetStaticMesh(Sphere.Object);
	}

	/* projectile movement component  */
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->MaxSpeed = 500.f;
	ProjectileMovementComponent->InitialSpeed = 500.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bIsHomingProjectile = false;
	ProjectileMovementComponent->HomingAccelerationMagnitude = 500.f;
}

void ASpaceShooterEnemy::BeginPlay()
{
	Super::BeginPlay();

	SetHomingTarget();
}

void ASpaceShooterEnemy::SetHomingTarget()
{
	/** assume single player  */
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (PlayerCharacter)
	{
		ProjectileMovementComponent->bIsHomingProjectile = FMath::RandBool();
		ProjectileMovementComponent->HomingTargetComponent = PlayerCharacter->GetCapsuleComponent();
		ProjectileMovementComponent->HomingAccelerationMagnitude = 500.f;
	}
}