// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpaceShooterEnemy.generated.h"

/**
 * 
 */
UCLASS()
class MAMMOTH_API ASpaceShooterEnemy : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* EnemyMesh;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovementComponent;
	
public:
	
	ASpaceShooterEnemy();

	virtual void BeginPlay() override;

	void SetHomingTarget();

	
	
private:

	/** damage to cause to player */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float BaseDamage = 20.f;
	
};
