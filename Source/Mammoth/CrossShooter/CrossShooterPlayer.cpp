// Fill out your copyright notice in the Description page of Project Settings.

#include "CrossShooterPlayer.h"
#include "ConstructorHelpers.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "CrossShooterProjectile.h"

// Sets default values
ACrossShooterPlayer::ACrossShooterPlayer()
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
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ConeTemplate(TEXT("StaticMesh'/Engine/BasicShapes/Cone.Cone'"));
	if (ConeTemplate.Succeeded())
	{
		ConeMesh->SetStaticMesh(ConeTemplate.Object);
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
	ProjectileSpawnPoint01 = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPoint01"));
	ProjectileSpawnPoint01->SetupAttachment(RootComponent);
	ProjectileSpawnPoint01->SetRelativeLocation(FVector(60.f, 0.f, 0.f));

	/* 02 arrow component  */
	ProjectileSpawnPoint02 = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPoint02"));
	ProjectileSpawnPoint02->SetupAttachment(RootComponent);
	ProjectileSpawnPoint02->SetRelativeLocation(FVector(-60.f, 0.f, 0.f));
	ProjectileSpawnPoint02->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));

	/* 03 arrow component  */
	ProjectileSpawnPoint03 = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPoint03"));
	ProjectileSpawnPoint03->SetupAttachment(RootComponent);
	ProjectileSpawnPoint03->SetRelativeLocation(FVector(0.f, 60.f, 0.f));
	ProjectileSpawnPoint03->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

	/* 04 arrow component  */
	ProjectileSpawnPoint04 = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPoint04"));
	ProjectileSpawnPoint04->SetupAttachment(RootComponent);
	ProjectileSpawnPoint04->SetRelativeLocation(FVector(0.f, -60.f, 0.f));
	ProjectileSpawnPoint04->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	/**  set the projectile class */
	static ConstructorHelpers::FClassFinder<ACrossShooterProjectile> ProjectileBPClass(TEXT("/Game/BP/CrossShooter/BP_CrossShooterProjectile"));
	if (ProjectileBPClass.Class)
	{
		ProjectileTemplate = ProjectileBPClass.Class;
	}

	/** class defaults  */
	bCanFire = true;
}

float ACrossShooterPlayer::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
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

// Called when the game starts or when spawned
void ACrossShooterPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnPoints.Add(ProjectileSpawnPoint01);
	SpawnPoints.Add(ProjectileSpawnPoint02);
	SpawnPoints.Add(ProjectileSpawnPoint03);
	SpawnPoints.Add(ProjectileSpawnPoint04);
}

// Called every frame
void ACrossShooterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACrossShooterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/** fire  */
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ACrossShooterPlayer::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ACrossShooterPlayer::StopFire);

	/** movement  */
	PlayerInputComponent->BindAxis("MoveUp", this, &ACrossShooterPlayer::MoveX);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACrossShooterPlayer::MoveY);
}

void ACrossShooterPlayer::MoveX(float Value)
{
	AddMovementInput(FVector(-1.f, 0.f, 0.f), Value);
}

void ACrossShooterPlayer::MoveY(float Value)
{
	AddMovementInput(FVector(0.f, 1.f, 0.f), Value);
}

void ACrossShooterPlayer::StartFire()
{
	if (bCanFire)
	{
		GetWorldTimerManager().SetTimer(FireTimer, this, &ACrossShooterPlayer::Fire, FireDelay, true, 0.f);
	}
	bCanFire = false;
}

void ACrossShooterPlayer::StopFire()
{
	GetWorldTimerManager().ClearTimer(FireTimer);
	
}

void ACrossShooterPlayer::Fire()
{
	for (UArrowComponent* CurrentSpawnPoint : SpawnPoints)
	{
		if (ProjectileTemplate && CurrentSpawnPoint)
		{
			UWorld* const World = GetWorld();
			if (World)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = Instigator; //  GetController();
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				
				FVector SpawnLocation = CurrentSpawnPoint->GetComponentLocation();
				FRotator SpawnRotation = CurrentSpawnPoint->GetComponentRotation();
				
				ACrossShooterProjectile* Spawned = World->SpawnActor<ACrossShooterProjectile>(ProjectileTemplate, SpawnLocation, SpawnRotation, SpawnParams);
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
	
	/** re-enable the fire ability  */
	FTimerDelegate ResetCanFireDelegate;
	ResetCanFireDelegate.BindLambda([&] { bCanFire = true;  });
	
	FTimerHandle ResetCanFireTimer;
	GetWorldTimerManager().SetTimer(ResetCanFireTimer, ResetCanFireDelegate, FireDelay, false);
}

