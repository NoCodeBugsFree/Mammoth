// Fill out your copyright notice in the Description page of Project Settings.

#include "MoveToPlayer.h"
#include "ConstructorHelpers.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "MoveToPickup.h"

// Sets default values
AMoveToPlayer::AMoveToPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/** mesh  */
	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	SphereMesh->SetupAttachment(RootComponent);

	/**  set the default static mesh */
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Sphere(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (Sphere.Succeeded())
	{
		SphereMesh->SetStaticMesh(Sphere.Object);
	}
}

// Called when the game starts or when spawned
void AMoveToPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		PC = PlayerController;
	}
}

// Called every frame
void AMoveToPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(PC)
	{
		/** find start line trace point - level camera location  */
		FVector Start;
		FRotator OutRotation; /** discarded  */
		PC->PlayerCameraManager->GetActorEyesViewPoint(Start, OutRotation);

		/** find end line trace point - mouse position in the world  */
		FVector WorldLocation; /** discarded  */
		FVector WorldDirection;
		PC->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);

		FVector End = WorldDirection * TraceDistance;
		
		FHitResult Hit;
		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(this);

		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_WorldStatic, CollisionQueryParams))
		{
			if (Hit.GetActor())
			{
				/** up at capsule half height  */
				FVector Target = Hit.Location + FVector(0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
				SetActorLocation(FMath::VInterpTo(GetActorLocation(), Target, DeltaTime, InterpSpeed));
			} 
		}
	}

}

void AMoveToPlayer::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	/** pick up the pick up  */
	if ( OtherActor && OtherActor != this && !OtherActor->IsPendingKill() )
	{
		if(AMoveToPickup* Pickup = Cast<AMoveToPickup>(OtherActor))
		{
			if (Pickup->GetActorScale3D().X < GetActorScale3D().X)
			{
				Score++;
				OnPickUp(true);
				SetActorScale3D(GetActorScale3D() * 1.05f);
				Pickup->SetActorEnableCollision(false);
				Pickup->SetActorHiddenInGame(true);
				Pickup->Destroy();
			} 
			else
			{
				OnPickUp(false);
				Score--;
				SetActorScale3D(GetActorScale3D() * 0.95f);
				Pickup->SetActorEnableCollision(false);
				Pickup->SetActorHiddenInGame(true);
				Pickup->Destroy();
			}

			if (Score > 20)
			{
				WinTheGame();
			} 
			else if(Score < -10)
			{
				LoseTheGame();
			}
		}
	}
}

