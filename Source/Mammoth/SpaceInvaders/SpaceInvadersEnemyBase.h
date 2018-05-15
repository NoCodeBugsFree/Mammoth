// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpaceInvadersEnemyBase.generated.h"

UCLASS()
class MAMMOTH_API ASpaceInvadersEnemyBase : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* SphereMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UMaterialInstanceDynamic* DynamicMaterial;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* ProjectileSpawnPoint;

public:
	
	// Sets default values for this character's properties
	ASpaceInvadersEnemyBase();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	/** called to fire a projectile */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void Fire();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	FTimerHandle FireTimer;

	/** projectile template  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ASpaceInvadersProjectileBase> ProjectileTemplate;

public:	
	
	
};
