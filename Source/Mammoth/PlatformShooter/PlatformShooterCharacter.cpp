// Fill out your copyright notice in the Description page of Project Settings.

#include "PlatformShooterCharacter.h"
#include "ConstructorHelpers.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "SpaceInvaders/SpaceInvadersProjectileBase.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APlatformShooterCharacter::APlatformShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Use only Yaw from the controller and ignore the rest of the rotation.
	bUseControllerRotationYaw = true;

	/** camera boom  */
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 1200.f;
	CameraBoom->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	CameraBoom->bAbsoluteRotation = true;

	/* camera component  */
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraBoom);
	CameraComponent->SetProjectionMode(ECameraProjectionMode::Orthographic);
	CameraComponent->SetOrthoWidth(3072.f);
	CameraComponent->bUsePawnControlRotation = false;

	/* 01 arrow component  */
	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPoint01"));
	ProjectileSpawnPoint->SetupAttachment(RootComponent);
	ProjectileSpawnPoint->SetRelativeLocation(FVector(60.f, 0.f, 0.f));

	/**  set the projectile class */
	static ConstructorHelpers::FClassFinder<ASpaceInvadersProjectileBase> ProjectileBPClass(TEXT("/Game/BP/PlatformShooter/BP_PlatformShooterProjectile"));
	if (ProjectileBPClass.Class)
	{
		ProjectileTemplate = ProjectileBPClass.Class;
	}

	// Prevent all automatic rotation behavior on the camera, character, and camera component
	
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Configure character movement
	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;

	// Lock character motion onto the XZ plane, so the character can't move in or out of the screen
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

	// Behave like a traditional 2D platformer character, with a flat bottom instead of a curved capsule bottom
	// Note: This can cause a little floating when going up inclines; you can choose the tradeoff between better
	// behavior on the edge of a ledge versus inclines by setting this to true or false
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;
}

// Called when the game starts or when spawned
void APlatformShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlatformShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Now setup the rotation of the controller based on the direction we are travelling
	const FVector PlayerVelocity = GetVelocity();
	float TravelDirection = PlayerVelocity.X;
	// Set the rotation so that the character faces his direction of travel.
	if (Controller != nullptr)
	{
		if (TravelDirection < 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
		}
		else if (TravelDirection > 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
		}
	}
}

// Called to bind functionality to input
void APlatformShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/** Fire  */
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlatformShooterCharacter::Fire);
	
	/** jump  */
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlatformShooterCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlatformShooterCharacter::StopJumping);

	/** movement  */
	PlayerInputComponent->BindAxis("MoveRight", this, &APlatformShooterCharacter::MoveRight);

}

void APlatformShooterCharacter::MoveRight(float Value)
{
	if (Value != 0.f)
	{
		// Apply the input to the character motion
		AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
	}
}

void APlatformShooterCharacter::Fire()
{
	if (ProjectileTemplate)
	{
		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
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

