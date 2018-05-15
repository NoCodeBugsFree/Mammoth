// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlatformShooterCharacter.generated.h"

UCLASS()
class MAMMOTH_API APlatformShooterCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

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

	// Sets default values for this character's properties
	APlatformShooterCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** projectile template  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ASpaceInvadersProjectileBase> ProjectileTemplate;
	
private:

	void MoveRight(float Value);
	
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void Fire();

public:	
	
	
	
};
