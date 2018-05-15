// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RollerBall3D_TPPawn.generated.h"

UCLASS()
class MAMMOTH_API ARollerBall3D_TPPawn : public APawn
{
	GENERATED_BODY()

	/** static mesh component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BallMesh;

	/** camera boom  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	
	/* camera component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;
	
public:

	// Sets default values for this pawn's properties
	ARollerBall3D_TPPawn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	int32 Score = 0;
	
protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	void MoveRight(float Value);

	void MoveForward(float Value);

	void Jump();

	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	// -----------------------------------------------------------------------------------

	/** Impulse to apply when we jump  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float JumpImpulse = 350000.0f;

	/** Torque to apply when trying to roll ball */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float RollTorque = 5000000.0f;

	/** Indicates whether we can currently jump, use to prevent double jumping */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	uint32 bCanJump : 1;

public:	
	
	
	
};
