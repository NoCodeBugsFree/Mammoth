// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceInvadersEnemyBase.h"
#include "ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "SpaceInvadersPlayer.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "SpaceInvadersProjectileBase.h"

// Sets default values
ASpaceInvadersEnemyBase::ASpaceInvadersEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	/** mesh  */
	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	SphereMesh->SetupAttachment(RootComponent);
	SphereMesh->SetCollisionProfileName("OverlapOnlyPawn");

	/* Projectile Spawn Points  */
	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPoint01"));
	ProjectileSpawnPoint->SetupAttachment(RootComponent);
	ProjectileSpawnPoint->SetRelativeLocation(FVector(60.f, 0.f, 0.f));

	GetCapsuleComponent()->SetCapsuleRadius(50.f);

	/**  set the default static mesh */
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereTemplate(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (SphereTemplate.Succeeded())
	{
		SphereMesh->SetStaticMesh(SphereTemplate.Object);
	}

	/**  set the projectile class */
	static ConstructorHelpers::FClassFinder<ASpaceInvadersProjectileBase> ProjectileBPClass(TEXT("/Game/BP/SpaceInvaders/BP_SpaceInvadersEnemyProjectile"));
	if (ProjectileBPClass.Class)
	{
		ProjectileTemplate = ProjectileBPClass.Class;
	}

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void ASpaceInvadersEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	DynamicMaterial = SphereMesh->CreateDynamicMaterialInstance(0);

	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue("Color", FLinearColor::MakeRandomColor());
	}
}

void ASpaceInvadersEnemyBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	/** kill player instantly  */
	if (OtherActor && OtherActor != this && !OtherActor->IsPendingKill() && !IsPendingKill() && OtherActor->IsA(ASpaceInvadersPlayer::StaticClass()))
	{
		UGameplayStatics::ApplyDamage(OtherActor, 500.f, GetController(), this, UDamageType::StaticClass());
		SetActorEnableCollision(false);
		SetActorHiddenInGame(true);
		Destroy();
	}
}

void ASpaceInvadersEnemyBase::Fire()
{
	if (ProjectileTemplate)
	{
		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator; //  GetController();
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
			FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();

			ASpaceInvadersProjectileBase* Spawned = World->SpawnActor<ASpaceInvadersProjectileBase>(ProjectileTemplate, SpawnLocation, SpawnRotation, SpawnParams);
			if (Spawned)
			{

			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ProjectileTemplate == NULL"));
	}
}
