// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "SpaceShooterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class MAMMOTH_API ASpaceShooterPlayer : public APaperCharacter
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* ProjectileSpawnPoint;
	
public:

	ASpaceShooterPlayer();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void AddScore() { Score++; }

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	/** called to play all impact FX (sound, camera shake, emitter particles) 
	*	when we hit by enemy
	*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Gameplay")
	void PlayImpactFX();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:

	void MoveRight(float Value);
	
	void Fire();

private:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ASpaceShooterProjectile> ProjectileClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	int32 Score = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float Health = 100.f;

public:
	
	FORCEINLINE int32 GetScore() const { return Score; }
	FORCEINLINE float GetHealth() const { return Health; }
};
