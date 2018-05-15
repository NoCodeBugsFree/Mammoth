// Fill out your copyright notice in the Description page of Project Settings.

#include "RotationalShooterPlayer.h"
#include "ConstructorHelpers.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "RotationalShooterProjectile.h"

ARotationalShooterPlayer::ARotationalShooterPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/** disable capsule gravity  */
	GetCapsuleComponent()->SetEnableGravity(false);

	/** disable skeletal mesh gravity and collision */
	GetMesh()->SetEnableGravity(false);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->bGenerateOverlapEvents = false;
	GetMesh()->BodyInstance.bNotifyRigidBodyCollision = false;

	/* scene component  */
	BodyScene = CreateDefaultSubobject<USceneComponent>(TEXT("BodyScene"));
	BodyScene->SetupAttachment(RootComponent);

	/** mesh  */
	TestConeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cone"));
	TestConeMesh->SetupAttachment(BodyScene);
	TestConeMesh->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	TestConeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TestConeMesh->bGenerateOverlapEvents = false;
	TestConeMesh->BodyInstance.bNotifyRigidBodyCollision = false;

	/**  set the default static mesh */
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ConeTemplate(TEXT("StaticMesh'/Engine/BasicShapes/Cone.Cone'"));
	if (ConeTemplate.Succeeded())
	{
		TestConeMesh->SetStaticMesh(ConeTemplate.Object);
	}

	/**  set the projectile class */
	static ConstructorHelpers::FClassFinder<ARotationalShooterProjectile> BPClass(TEXT("/Game/BP/RotationalShooter/BP_RotationalShooterProjectile"));
	if (BPClass.Class)
	{
		ProjectileTemplate = BPClass.Class;
	}

	/* arrow component  */
	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(TestConeMesh);
	ProjectileSpawnPoint->ArrowColor = FColor::Green;
	ProjectileSpawnPoint->ArrowSize = 3.f;
	ProjectileSpawnPoint->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
	ProjectileSpawnPoint->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));

	/** camera boom  */
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 1200.f;
	CameraBoom->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	
	/* camera component  */
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraBoom);

	/** mesh  */
	ReticleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	ReticleMesh->SetupAttachment(RootComponent);
	ReticleMesh->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.25f));
	ReticleMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	ReticleMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ReticleMesh->bGenerateOverlapEvents = false;

	/**  set the default static mesh */
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Sphere(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (Sphere.Succeeded())
	{
		ReticleMesh->SetStaticMesh(Sphere.Object);
	}
}

float ARotationalShooterPlayer::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
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

void ARotationalShooterPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	/** setup player controller  */
	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		PlayerController = PC;
		PlayerController->bShowMouseCursor = true;
		PlayerController->CurrentMouseCursor = EMouseCursor::Crosshairs;
	}
}

void ARotationalShooterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateToMouseCursor(DeltaTime);
}

void ARotationalShooterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	/** fire  */
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ARotationalShooterPlayer::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ARotationalShooterPlayer::StopFire);

	/** movement  */
	PlayerInputComponent->BindAxis("MoveRight", this, &ARotationalShooterPlayer::MovementX);
	PlayerInputComponent->BindAxis("MoveUp", this, &ARotationalShooterPlayer::MovementY);
}

void ARotationalShooterPlayer::MovementX(float Value)
{
	AddMovementInput(FVector(0.f, Value, 0.f));
}

void ARotationalShooterPlayer::MovementY(float Value)
{
	AddMovementInput(FVector(-Value, 0.f, 0.f));
}

void ARotationalShooterPlayer::RotateToMouseCursor(float DeltaTime)
{
	if (PlayerController)
	{
		float MouseX, MouseY;
		FVector WorldLocation, WorldDirection;

		PlayerController->GetMousePosition(MouseX, MouseY);
		PlayerController->DeprojectScreenPositionToWorld(MouseX, MouseY, WorldLocation, WorldDirection);

		/** setup reticle location and opacity */
		float RoughtStickMagnitude = FMath::Max(FMath::Abs(WorldDirection.X), FMath::Abs(WorldDirection.Y));

		FVector VectorToRotate = FVector(WorldDirection.X, -WorldDirection.Y, 0.f);
		FRotator Rotation = FRotationMatrix::MakeFromX(VectorToRotate).Rotator();

		FVector ReticleDistanceVector = FVector(ReticleDistance, 0.f, 0.f);

		/** find start line trace point - level camera location  */
		FVector Start; FRotator OutRotation;
		PlayerController->PlayerCameraManager->GetActorEyesViewPoint(Start, OutRotation);
			
		/** find end line trace point - mouse position in the world  */
		PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);

		FRotator RotatorToDirection = FRotationMatrix::MakeFromX(WorldLocation - Start).Rotator();
		FVector Direction = RotatorToDirection.Vector();
		FVector End = Direction * 10000.f;

			
		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(this);

		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_WorldStatic, CollisionQueryParams))
		{
			if (Hit.GetActor())
			{
				float RawReticleDistance = FMath::Abs((GetActorLocation() - Hit.Location).Size()) - 100.f;
				float CurrentReticleDistance = RawReticleDistance > ReticleDistance ? RawReticleDistance : ReticleDistance;
				ReticleDistanceVector = FVector(CurrentReticleDistance, 0.f, 0.f);
			}
		}	

		ReticleMesh->SetRelativeLocation(Rotation.UnrotateVector(ReticleDistanceVector));

		/** rotate wings towards cursor  */
		FVector From = ReticleMesh->RelativeLocation;
		FVector To = BodyScene->RelativeLocation;
		FRotator WingsDesiredRotatation = FRotationMatrix::MakeFromX((To - From).GetSafeNormal()).Rotator();

		FRotator RInterpToRotator = FMath::RInterpTo(BodyScene->RelativeRotation, WingsDesiredRotatation, DeltaTime, InterpSpeed);
		BodyScene->SetRelativeRotation(UKismetMathLibrary::RLerp(BodyScene->RelativeRotation, RInterpToRotator, RoughtStickMagnitude, true));
	}
}

void ARotationalShooterPlayer::StartFire()
{
	GetWorldTimerManager().SetTimer(FireTimer, this, &ARotationalShooterPlayer::Fire, FireRate, true, 0.f);
}

void ARotationalShooterPlayer::StopFire()
{
	GetWorldTimerManager().ClearTimer(FireTimer);
}

void ARotationalShooterPlayer::Fire()
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
			
			ARotationalShooterProjectile* Spawned = World->SpawnActor<ARotationalShooterProjectile>(ProjectileTemplate, SpawnLocation, SpawnRotation, SpawnParams);
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

