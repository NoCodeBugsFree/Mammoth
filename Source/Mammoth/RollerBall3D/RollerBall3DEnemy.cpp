// Fill out your copyright notice in the Description page of Project Settings.

#include "RollerBall3DEnemy.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/EngineTypes.h"
#include "RollerBall3DPickup.h"
#include "DrawDebugHelpers.h"

void ARollerBall3DEnemy::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle FindPickupTimer;
	GetWorldTimerManager().SetTimer(FindPickupTimer, this, &ARollerBall3DEnemy::SetDestination, 0.1f, true);
}

void ARollerBall3DEnemy::SetDestination()
{
	TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_PhysicsBody));
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	
	TArray<AActor*> OutActors;
	
	float MinDistance = MAX_FLT;

	if (UKismetSystemLibrary::SphereOverlapActors(this, GetActorLocation(), Radius, ObjectTypes, ARollerBall3DPickup::StaticClass(), ActorsToIgnore, OutActors))
	{
		//DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 36, FColor::Green, false, 1.f, 0, 20.f);
		for (AActor* Pickup : OutActors)
		{
			if (Pickup)
			{
				FVector PickupLocation = Pickup->GetActorLocation();
				float CurrentDistance = (PickupLocation - GetActorLocation()).Size();
				if (CurrentDistance < MinDistance)
				{
					MinDistance = CurrentDistance;
					Destination = PickupLocation;
				}
			}
		}
	}
	else
	{
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
		if (PlayerPawn)
		{
			Destination = PlayerPawn->GetActorLocation();
		}
		else
		{
			Destination = FVector(0.f, 0.f, 0.f);
		}
	}
}
