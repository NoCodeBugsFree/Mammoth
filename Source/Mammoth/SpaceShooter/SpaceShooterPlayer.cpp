// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceShooterPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "ConstructorHelpers.h"
#include "SpaceShooterProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "SpaceShooterEnemy.h"

ASpaceShooterPlayer::ASpaceShooterPlayer()
{
	/** camera boom  */
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 500.f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = false;
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->CameraLagSpeed = 2.f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->SetWorldRotation(FRotator(-90.f, 0.0f, 0.0f));
	SpringArm->SetRelativeLocation(FVector(750.f, 0.f, 0.f));
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;

	/** camera  */
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	// flat view
	Camera->ProjectionMode = ECameraProjectionMode::Orthographic;
	Camera->OrthoWidth = 3072.f; // 1024.f by default
	Camera->AspectRatio = 3.f / 4.f;
	Camera->SetWorldRotation(FRotator(-90.f, 0.0f, 0.0f));

	// disable auto exposure
	Camera->PostProcessSettings.bOverride_AutoExposureMaxBrightness = true;
	Camera->PostProcessSettings.bOverride_AutoExposureMinBrightness = true;
	Camera->PostProcessSettings.AutoExposureMinBrightness = 1.f;
	Camera->PostProcessSettings.AutoExposureMaxBrightness = 1.f;

	//  disable motion blur
	Camera->PostProcessSettings.bOverride_MotionBlurAmount = true;
	Camera->PostProcessSettings.MotionBlurAmount = 0.f;
	
	/* arrow component  */
	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(RootComponent);

	/**  set the projectile class */
	static ConstructorHelpers::FClassFinder<ASpaceShooterProjectile> ProjectileClassTemplate(TEXT("/Game/BP/SpaceShooter/BP_SpaceShooterProjectile"));
	if (ProjectileClassTemplate.Class)
	{
		ProjectileClass = ProjectileClassTemplate.Class;
	}
}

void ASpaceShooterPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/** Fire  */
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASpaceShooterPlayer::Fire);

	/** MoveRight  */
	PlayerInputComponent->BindAxis("MoveRight", this, &ASpaceShooterPlayer::MoveRight);
}

float ASpaceShooterPlayer::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (ActualDamage > 0.f)
	{
		Health -= ActualDamage;
		// If the damage depletes our health set our lifespan to zero - which will destroy the actor  
		if (Health <= 0.f)
		{
			SetLifeSpan(0.001f); /** TODO game over  */
		}
	}

	return ActualDamage;
}

void ASpaceShooterPlayer::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor && OtherActor != this && !OtherActor->IsPendingKill())
	{
		if (ASpaceShooterEnemy* Enemy = Cast<ASpaceShooterEnemy>(OtherActor))
		{
			PlayImpactFX();

			UGameplayStatics::ApplyDamage(this, 20.f, nullptr, OtherActor, UDamageType::StaticClass());

			Enemy->Destroy();
		}
	}
}

void ASpaceShooterPlayer::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

void ASpaceShooterPlayer::Fire()
{
	if (ProjectileClass)
	{
		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator; //  GetController();
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			
			FTransform SpawnTransform = ProjectileSpawnPoint->GetComponentTransform();
			
			ASpaceShooterProjectile* Spawned = World->SpawnActor<ASpaceShooterProjectile>(ProjectileClass, SpawnTransform, SpawnParams);
			if (Spawned)
			{
			
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ProjectileClass == NULL"));
	}
}
