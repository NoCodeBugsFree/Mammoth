// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceInvadersProjectileBase.h"
#include "SpaceInvadersEnemyBase.h"
#include "SpaceInvadersPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "SpaceInvadersMovementManager.h"

// Sets default values
ASpaceInvadersProjectileBase::ASpaceInvadersProjectileBase()
{
 	

}

// Called when the game starts or when spawned
void ASpaceInvadersProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASpaceInvadersProjectileBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// Other Actor is the actor that triggered the event. Check that is not ourself. 
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && (!OtherActor->IsPendingKill()))
	{
		/** kill the enemy instantly  */
		if (ASpaceInvadersEnemyBase* Enemy = Cast<ASpaceInvadersEnemyBase>(OtherActor))
		{
			/** disable enemy  */
			Enemy->SetActorEnableCollision(false);
			Enemy->SetActorHiddenInGame(true);

			/** remove enemy from movement manager  */
			if(ASpaceInvadersMovementManager* Manager = Cast<ASpaceInvadersMovementManager>(Enemy->GetOwner()))
			{
				Manager->CheckOutEnemy(Enemy);
			}

			/** mark as pending kill  */
			Enemy->Destroy();

			/** add killer's score  */
			if (ASpaceInvadersPlayer* Player = Cast<ASpaceInvadersPlayer>(GetOwner()))
			{
				Player->Score++;
			}
		} /** damage the player  */
		else if (ASpaceInvadersPlayer* Player = Cast<ASpaceInvadersPlayer>(OtherActor))
		{
			UGameplayStatics::ApplyDamage(Player, 20.f, nullptr, this, UDamageType::StaticClass());
		}

		/** disable and destroy the projectile  */
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		Destroy();
	}
}

