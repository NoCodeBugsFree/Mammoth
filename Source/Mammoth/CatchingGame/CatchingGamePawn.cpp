// Fill out your copyright notice in the Description page of Project Settings.

#include "CatchingGamePawn.h"
#include "ConstructorHelpers.h"
#include "CatchingGamePickup.h"

// Sets default values
ACatchingGamePawn::ACatchingGamePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/** mesh  */
	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	CubeMesh->SetupAttachment(RootComponent);
	CubeMesh->SetSimulatePhysics(true);
	CubeMesh->BodyInstance.bNotifyRigidBodyCollision = true;
	CubeMesh->SetLinearDamping(0.8f);
	CubeMesh->SetAngularDamping(0.8f);
	SetRootComponent(CubeMesh);

	/**  set the default static mesh */
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Sphere(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (Sphere.Succeeded())
	{
		CubeMesh->SetStaticMesh(Sphere.Object);
	}

	/** class defaults  */
	bCanJump = true;
}

// Called when the game starts or when spawned
void ACatchingGamePawn::BeginPlay()
{
	Super::BeginPlay();

	/** save the forward and right vectors  */
	ForwardVector = GetActorForwardVector();
	RightVector = GetActorRightVector();
}

// Called every frame
void ACatchingGamePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/** calc summary force direction */
	CurrentForceDirection = CurrentRightVector + CurrenForwardVector;
	CurrentForceDirection.Normalize();

	/** times magnitude  */
	FVector Force = CurrentForceDirection * MovementForce;

	/** apply force  */
	CubeMesh->AddForce(Force, NAME_None, true);
}

// Called to bind functionality to input
void ACatchingGamePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/** movement  */
	PlayerInputComponent->BindAxis("MoveRight", this, &ACatchingGamePawn::MoveRight);
	PlayerInputComponent->BindAxis("MoveUp", this, &ACatchingGamePawn::MoveUp);
	
	/** Jump  */
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACatchingGamePawn::Jump);
}

void ACatchingGamePawn::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor && OtherActor != this && !OtherActor->IsPendingKill() && OtherActor->IsA(ACatchingGamePickup::StaticClass()))
	{
		Score++;
		OtherActor->Destroy();
	}
}

void ACatchingGamePawn::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if (Other && Other != this && !IsPendingKill() && !Other->IsPendingKill() && Other->ActorHasTag("Floor"))
	{
		bCanJump = true;
	}
}

void ACatchingGamePawn::Jump()
{
	if (bCanJump)
	{
		/** disable multi jump  */
		bCanJump = false;

		/**  throw up a pawn  */
		FVector Impulse = CubeMesh->GetMass() * FVector(FMath::RandRange(0.f, 0.2f), FMath::RandRange(0.f, 0.2f), 1.f) * FMath::RandRange(ImpulseStrengthMin, ImpulseStrengthMax);
		FVector Location = CubeMesh->GetCenterOfMass();
		CubeMesh->AddImpulseAtLocation(Impulse, Location);

		PlayJumpFX();
	}
}

void ACatchingGamePawn::MoveRight(float Value)
{
	/** stop the movement in left/right direction */
	if (Value == 0.f)
	{
		CurrenForwardVector = FVector::ZeroVector;
		return;
	}

	/** left or right */
	if (Value > 0)
	{
		CurrenForwardVector = ForwardVector;
	}
	else
	{
		CurrenForwardVector = -ForwardVector;
	}
}

void ACatchingGamePawn::MoveUp(float Value)
{
	/** stop the movement in up/down direction */
	if (Value == 0.f)
	{
		CurrentRightVector = FVector::ZeroVector;
		return;
	}

	/** up or down */
	if (Value > 0)
	{
		CurrentRightVector = RightVector;
	}
	else
	{
		CurrentRightVector = -RightVector;
	}
}
