// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteActor.h"
#include "SpaceShooterProjectile.generated.h"

/**
 * 
 */
UCLASS()
class MAMMOTH_API ASpaceShooterProjectile : public APaperSpriteActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovementComponent;
	
public:
	
	ASpaceShooterProjectile();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Gameplay")
	void PlayImpactFX();
};
