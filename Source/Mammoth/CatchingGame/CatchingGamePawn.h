// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CatchingGamePawn.generated.h"

UCLASS()
class MAMMOTH_API ACatchingGamePawn : public APawn
{
	GENERATED_BODY()

protected:

	/** static mesh component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* CubeMesh;
	
public:

	// Sets default values for this pawn's properties
	ACatchingGamePawn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/** called to handle pickup event  */
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	/** called to re-enable jump ability  */
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	
	/** called to  */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "AAA")
	void PlayJumpFX();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void Jump();

	void MoveRight(float Value);

	void MoveUp(float Value);

	// -----------------------------------------------------------------------------------

	/** current score amount  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	int32 Score = 0;

	/** shows whether is  or not  */
	UPROPERTY(BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	uint32 bCanJump : 1;
	
	/* the min power of impulse that throw up a bot when it jumps */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float ImpulseStrengthMin = 600.f;

	/* the max power of impulse that throw up a bot when it jumps */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float ImpulseStrengthMax = 600.f;

	/** the strength of force to apply to this actor to move */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float MovementForce = 2000.f;

	/** forward movement vector for our pawn. Will be setted at BeginPlay as GetActorForwardVector()  */
	UPROPERTY(BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	FVector ForwardVector;

	/** right movement vector for our pawn. Will be setted at BeginPlay as GetActorForwardVector()  */
	UPROPERTY(BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	FVector RightVector;

	/** each frame calculated force to apply summary vector */
	UPROPERTY()
	FVector CurrentForceDirection;

	/** each frame calculated force to apply forward vector */
	UPROPERTY()
	FVector CurrenForwardVector;

	/** each frame calculated force to apply right vector */
	UPROPERTY()
	FVector CurrentRightVector;
	
public:	

	
};
