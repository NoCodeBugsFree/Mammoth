// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RotationalShooterPlayer.generated.h"

UCLASS()
class MAMMOTH_API ARotationalShooterPlayer : public ACharacter
{
	GENERATED_BODY()

	/** static mesh component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* TestConeMesh;

	/* arrow component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* ProjectileSpawnPoint;

	/** camera boom  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	
	/* camera component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

	/** static mesh component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ReticleMesh;
	
	/* scene component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* BodyScene;
	
public:

	// Sets default values for this character's properties
	ARotationalShooterPlayer();

	/** current score  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	int32 Score = 0;

	/** Apply damage to this actor.*/
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	/** the distance in uu (cm) from player to reticle  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float ReticleDistance = 200.f;

	/** current player's health  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float Health = 100.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	FHitResult Hit;

	/** calls to move X-axis  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void MovementX(float Value);

	/** calls to move Y-axis  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void MovementY(float Value);

	/** [tick] calls to rotate weapon to mouse cursor  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void RotateToMouseCursor(float DeltaTime);

	/** calls to start firing as fast as we can  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void StartFire();

	/** calls to stop firing  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void StopFire();

	/** internal call for instant fire */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void Fire();

	// -----------------------------------------------------------------------------------
	
	/** timer that handles fire logic  */
	UPROPERTY()
	FTimerHandle FireTimer;

	/** the delay (in sec) between shoots  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float FireRate = 0.25f;

	/** interp rotation speed from current rotation to desired */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float InterpSpeed = 10.f;
	
	/** projectile template to spawn when fire  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ARotationalShooterProjectile> ProjectileTemplate;

	/** player controller reference  */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class APlayerController* PlayerController;

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
};
