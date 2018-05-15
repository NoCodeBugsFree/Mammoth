// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RotationalShooterEnemy.generated.h"

UCLASS()
class MAMMOTH_API ARotationalShooterEnemy : public ACharacter
{
	GENERATED_BODY()

	/** static mesh component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* TestSphereMesh;

	/** mesh DMI  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UMaterialInstanceDynamic* DynamicMaterial;

	/* sphere collision  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* KillSphere;
	
public:

	// Sets default values for this character's properties
	ARotationalShooterEnemy();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:

	/* calls when kill sphere component overlaps */
	UFUNCTION()
	void OnKillSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	/** player pawn reference - enemy for this AI  */
	UPROPERTY(Transient)
	class APawn* TargetPawn;

};
