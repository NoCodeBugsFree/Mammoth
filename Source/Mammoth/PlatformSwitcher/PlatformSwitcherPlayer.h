// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CatchingGame/CatchingGamePawn.h"
#include "PlatformSwitcherPlayer.generated.h"

/**
 * 
 */
UCLASS()
class MAMMOTH_API APlatformSwitcherPlayer : public ACatchingGamePawn
{
	GENERATED_BODY()
	
	/** camera boom  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	
	/* camera component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;
	
public:

	APlatformSwitcherPlayer();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** called to win the game */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "AAA")
	void Win();
	
	/** called to lose the game */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "AAA")
	void Lose();

	/** called to play teleport FX */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "AAA")
	void PlayTeleportFX();

protected:

	virtual void BeginPlay() override;

	void TeleportToFirstLine();
	
	void TeleportToSecondLine();

	/** re-enable teleport ability  */
	void ResetTeleport();

private:

	/** shows whether we can teleport right now or not */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	uint32 bCanTeleport : 1;

	/** and 1st line x  */
	UPROPERTY(BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	FVector StartLocation;

	/** and 1st line x  */
	UPROPERTY(BlueprintReadWrite, Category = "Config", meta = (AllowPrivateAccess = "true"))
	FVector SecondLine;
	
};
