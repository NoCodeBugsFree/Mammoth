// Fill out your copyright notice in the Description page of Project Settings.

#include "RollerBall3D_TPPawn.h"
#include "ConstructorHelpers.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "RollerBall3D_TPPickup.h"

// Sets default values
ARollerBall3D_TPPawn::ARollerBall3D_TPPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	/** mesh  */
	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(BallMesh);
	BallMesh->SetCollisionProfileName("PhysicsActor");
	BallMesh->SetSimulatePhysics(true);
	BallMesh->SetLinearDamping(0.1f);
	BallMesh->SetAngularDamping(0.1f);
	BallMesh->BodyInstance.MassScale = 3.5f;
	BallMesh->BodyInstance.MaxAngularVelocity = 800.0f;
	BallMesh->SetNotifyRigidBodyCollision(true);

	/**  set the default static mesh */
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Sphere(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (Sphere.Succeeded())
	{
		BallMesh->SetStaticMesh(Sphere.Object);
	}
	
	/** camera boom  */
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 1200.f;
	CameraBoom->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	CameraBoom->bEnableCameraLag = false;
	CameraBoom->CameraLagSpeed = 3.f;
	CameraBoom->bAbsoluteRotation = true; // Rotation of the ball should not affect rotation of boom

	/* camera component  */
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraBoom);

	/** class defaults  */
	bCanJump = true;
}

// Called when the game starts or when spawned
void ARollerBall3D_TPPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARollerBall3D_TPPawn::MoveRight(float Value)
{
	const FVector Torque = FVector(-Value * RollTorque, 0.f, 0.f);
	BallMesh->AddTorqueInRadians(Torque);
}

void ARollerBall3D_TPPawn::MoveForward(float Value)
{
	const FVector Torque = FVector(0.f, -Value * RollTorque, 0.f);
	BallMesh->AddTorqueInRadians(Torque);
}

void ARollerBall3D_TPPawn::Jump()
{
	if (bCanJump)
	{
		const FVector Impulse = FVector(0.f, 0.f, JumpImpulse);
		BallMesh->AddImpulse(Impulse);
		bCanJump = false;
	}
}

void ARollerBall3D_TPPawn::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	bCanJump = true;
}

void ARollerBall3D_TPPawn::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	/** pickup logic here  */
	if (OtherActor && OtherActor != this && !OtherActor->IsPendingKill() && OtherActor->IsA(ARollerBall3D_TPPickup::StaticClass()))
	{
		OtherActor->Destroy();
		Score++;
	}
}

// Called every frame
void ARollerBall3D_TPPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARollerBall3D_TPPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/** movement  */
	PlayerInputComponent->BindAxis("MoveRight", this, &ARollerBall3D_TPPawn::MoveRight);
	PlayerInputComponent->BindAxis("MoveUp", this, &ARollerBall3D_TPPawn::MoveForward);

	/** jump  */
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ARollerBall3D_TPPawn::Jump);
}

