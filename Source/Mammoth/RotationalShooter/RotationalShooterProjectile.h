// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RotationalShooterProjectile.generated.h"

UCLASS()
class MAMMOTH_API ARotationalShooterProjectile : public AActor
{
	GENERATED_BODY()
	
	/* sphere root collision component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SphereCollision;

	/** static mesh component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ProjectileMesh;
	
	/** projectile movement component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovementComponent;

public:	

	// Sets default values for this actor's properties
	ARotationalShooterProjectile();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
};
