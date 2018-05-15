// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RollerBall3DPlayer.generated.h"

UCLASS()
class MAMMOTH_API ARollerBall3DPlayer : public APawn
{
	GENERATED_BODY()

	/** static mesh component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* SphereMesh;

public:

	// Sets default values for this pawn's properties
	ARollerBall3DPlayer();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/** called to handle pickup event  */
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	/** called to set new destination to move  */
	void LineTrace();

	/** [Tick] called to move this actor to desired position in the world  */
	void MoveToDestination();

	// -----------------------------------------------------------------------------------

	/** point in the world to move now  */
	UPROPERTY(BlueprintReadWrite, Category = "Config", meta = (AllowPrivateAccess = "true"))
	FVector Destination;

	/** the strength of force to apply to this actor to move */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float MovementForce = 500.f;

	/** current score amount  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	int32 Score = 0;

	/** the strength of force to apply to this actor to jump */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float JumpImpulseStrength = 600.f;

	/** player controller reference  */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class APlayerController* PlayerController;

public:	
	
	
	
};
