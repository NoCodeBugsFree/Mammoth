// Fill out your copyright notice in the Description page of Project Settings.

#include "RollerBall3DPlayer.h"
#include "RollerBall3DPickup.h"
#include "DrawDebugHelpers.h"

// Sets default values
ARollerBall3DPlayer::ARollerBall3DPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/** mesh  */
	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	SphereMesh->SetupAttachment(RootComponent);
	SphereMesh->SetSimulatePhysics(true);
	SphereMesh->BodyInstance.bNotifyRigidBodyCollision = true;
	SphereMesh->SetLinearDamping(0.8f);
	SphereMesh->SetAngularDamping(0.8f);
	SphereMesh->SetCollisionResponseToAllChannels(ECR_Block);
	SphereMesh->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
	SetRootComponent(SphereMesh);

	/**  set the default static mesh */
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Sphere(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (Sphere.Succeeded())
	{
		SphereMesh->SetStaticMesh(Sphere.Object);
	}
}

// Called when the game starts or when spawned
void ARollerBall3DPlayer::BeginPlay()
{
	Super::BeginPlay();

	Destination = GetActorLocation();
	
	/** setup player controller  */
	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		PlayerController = PC;
		PlayerController->bShowMouseCursor = true;
		PlayerController->CurrentMouseCursor = EMouseCursor::Crosshairs;
	}
}

// Called every frame
void ARollerBall3DPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveToDestination();
}

// Called to bind functionality to input
void ARollerBall3DPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/** Line Trace  */
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ARollerBall3DPlayer::LineTrace);
}

void ARollerBall3DPlayer::LineTrace()
{
	if (PlayerController)
	{
		/** find start line trace point - level camera location  */
		FVector Start;
		FRotator OutRotation; /** discarded  */
		PlayerController->PlayerCameraManager->GetActorEyesViewPoint(Start, OutRotation);

		/** find end line trace point - mouse position in the world  */
		FVector WorldLocation; /** discarded  */
		FVector WorldDirection;
		PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);

		FVector End = WorldDirection * 100000.f;
		
		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(this);
		FHitResult Hit;

		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, CollisionQueryParams))
		{
			Destination = Hit.Location;
		}

		// DrawDebugLine(GetWorld(), Start, End, FColor::Red, true, 100.f, (uint8)0, 10.f);
	}
}

void ARollerBall3DPlayer::MoveToDestination()
{
	FVector ForceDirection = Destination - GetActorLocation();
	ForceDirection.Normalize();

	FVector Force = ForceDirection * MovementForce;
	SphereMesh->AddForce(Force, NAME_None, true);
}

void ARollerBall3DPlayer::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor && OtherActor != this && !OtherActor->IsPendingKill() && OtherActor->IsA(ARollerBall3DPickup::StaticClass()))
	{
		Score++;
		OtherActor->Destroy();
	}
}

