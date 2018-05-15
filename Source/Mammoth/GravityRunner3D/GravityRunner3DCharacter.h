// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GravityRunner3DCharacter.generated.h"

UCLASS()
class MAMMOTH_API AGravityRunner3DCharacter : public ACharacter
{
	GENERATED_BODY()

	/** static mesh component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* SphereMesh;

	/** camera boom  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/* camera component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

public:

	// Sets default values for this character's properties
	AGravityRunner3DCharacter();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/** handle collisions with pickups and enemies  */
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** flipflop the character's gravity  */
	void ChangeGravity();

private:



public:	
	
	
};
