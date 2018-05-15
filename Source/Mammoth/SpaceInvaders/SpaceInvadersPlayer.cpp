// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceInvadersPlayer.h"
#include "ConstructorHelpers.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "SpaceInvadersProjectileBase.h"

// Sets default values
ASpaceInvadersPlayer::ASpaceInvadersPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	/** mesh  */
	ConeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ConeMesh"));
	ConeMesh->SetupAttachment(RootComponent);
	ConeMesh->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	ConeMesh->bGenerateOverlapEvents = false;
	ConeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	/**  set the default static mesh */
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereTemplate(TEXT("StaticMesh'/Engine/BasicShapes/Cone.Cone'"));
	if (SphereTemplate.Succeeded())
	{
		ConeMesh->SetStaticMesh(SphereTemplate.Object);
	}

	/** camera boom  */
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 1200.f;
	CameraBoom->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));

	/* camera component  */
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraBoom);

	/* 01 arrow component  */
	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPoint01"));
	ProjectileSpawnPoint->SetupAttachment(RootComponent);
	ProjectileSpawnPoint->SetRelativeLocation(FVector(60.f, 0.f, 0.f));

	/**  set the projectile class */
	static ConstructorHelpers::FClassFinder<ASpaceInvadersProjectileBase> ProjectileBPClass(TEXT("/Game/BP/SpaceInvaders/BP_SpaceInvadersProjectile"));
	if (ProjectileBPClass.Class)
	{
		ProjectileTemplate = ProjectileBPClass.Class;
	}
}

// Called when the game starts or when spawned
void ASpaceInvadersPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASpaceInvadersPlayer::MoveY(float Value)
{
	AddMovementInput(FVector(0.f, 1.f, 0.f), Value);
}

void ASpaceInvadersPlayer::StartFire()
{
	GetWorldTimerManager().SetTimer(FireTimer, this, &ASpaceInvadersPlayer::Fire, FireDelay, true, 0.f);
}

void ASpaceInvadersPlayer::StopFire()
{
	GetWorldTimerManager().ClearTimer(FireTimer);
}

void ASpaceInvadersPlayer::Fire()
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

// Called to bind functionality to input
void ASpaceInvadersPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/** fire  */
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASpaceInvadersPlayer::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASpaceInvadersPlayer::StopFire);

	/** movement  */
	PlayerInputComponent->BindAxis("MoveRight", this, &ASpaceInvadersPlayer::MoveY);
}

float ASpaceInvadersPlayer::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (ActualDamage > 0.f)
	{
		Health -= ActualDamage;
		// If the damage depletes our health set our lifespan to zero - which will destroy the actor  
		if (Health <= 0.f)
		{
			SetLifeSpan(0.001f);
		}
	}

	return ActualDamage;
}

