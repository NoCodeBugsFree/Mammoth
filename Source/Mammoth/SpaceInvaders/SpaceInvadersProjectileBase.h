// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CrossShooter/CrossShooterProjectile.h"
#include "SpaceInvadersProjectileBase.generated.h"

UCLASS()
class MAMMOTH_API ASpaceInvadersProjectileBase : public ACrossShooterProjectile
{
	GENERATED_BODY()

	
public:	
	// Sets default values for this actor's properties
	ASpaceInvadersProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* calls when sphere component overlaps a Pawn */
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;


public:	


	
	
};
