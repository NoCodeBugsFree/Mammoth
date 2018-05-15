// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpaceInvadersPlayer.generated.h"

UCLASS()
class MAMMOTH_API ASpaceInvadersPlayer : public ACharacter
{
	GENERATED_BODY()

protected:

	/** static mesh component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ConeMesh;

	/** camera boom  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/* camera component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

	/* Projectile Spawn Points  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* ProjectileSpawnPoint;

public:

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// Sets default values for this character's properties
	ASpaceInvadersPlayer();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Config", meta = (AllowPrivateAccess = "true"))
	int32 Score = 0;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** delay between shoots  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float FireDelay = 1.f;

	void MoveY(float Value);
	
	void StartFire();
	void StopFire();
	void Fire();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float Health = 100.f;

	UPROPERTY()
	FTimerHandle FireTimer;

	/** projectile template  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ASpaceInvadersProjectileBase> ProjectileTemplate;


public:	
	

	
	
};
