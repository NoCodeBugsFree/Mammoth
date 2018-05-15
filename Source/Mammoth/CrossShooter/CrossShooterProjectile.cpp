// Fill out your copyright notice in the Description page of Project Settings.

#include "CrossShooterProjectile.h"
#include "ConstructorHelpers.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "CrossShooterEnemy.h"
#include "CrossShooterPlayer.h"

// Sets default values
ACrossShooterProjectile::ACrossShooterProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	/** mesh  */
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	SetRootComponent(ProjectileMesh);
	ProjectileMesh->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
	ProjectileMesh->bGenerateOverlapEvents = false;
	ProjectileMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	/**  set the default static mesh */
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Sphere(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (Sphere.Succeeded())
	{
		ProjectileMesh->SetStaticMesh(Sphere.Object);
	}

	/* sphere collision  */
	ProjectileCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ProjectileCollision"));
	ProjectileCollision->SetupAttachment(RootComponent);
	ProjectileCollision->SetSphereRadius(55.f);
	ProjectileCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProjectileCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	ProjectileCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	/* projectile movement component  */
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 1500.f;
	ProjectileMovementComponent->MaxSpeed = 1500.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;

	InitialLifeSpan = 4.f;
}

// Called when the game starts or when spawned
void ACrossShooterProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileCollision->OnComponentBeginOverlap.AddDynamic(this, &ACrossShooterProjectile::OnOverlapBegin);
}

void ACrossShooterProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// Other Actor is the actor that triggered the event. Check that is not ourself. 
	if ( (OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && ( ! OtherActor->IsPendingKill()) )
	{
		if(ACrossShooterEnemy* Enemy = Cast<ACrossShooterEnemy>(OtherActor))
		{
			/** disable enemy  */
			Enemy->SetActorEnableCollision(false);
			Enemy->SetActorHiddenInGame(true);

			/** mark as pending kill  */
			Enemy->Destroy();

			/** add killer's score  */
			if(ACrossShooterPlayer* Player = Cast<ACrossShooterPlayer>(GetOwner()))
			{
				Player->Score++;
			}
		}

		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		Destroy();
	}
}

