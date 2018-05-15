// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpaceInvaders/SpaceInvadersPlayer.h"
#include "GravityPlatformerPlayer.generated.h"

/**
 * 
 */
UCLASS()
class MAMMOTH_API AGravityPlatformerPlayer : public ASpaceInvadersPlayer
{
	GENERATED_BODY()
	
public:

	AGravityPlatformerPlayer();

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/** use to prevent gravity change when we are in air 
	*	@param bCanChangeGravity flag used to handle change gravity 
	*/
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	
	/** handle collisions with pickups and enemies  */
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "AAA")
	void WinTheGame();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "AAA")
	void LoseTheGame();
	
	/** time to collect all pickups in the level or die  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	int32 LevelTime = 40;

	/** shows how many pickups we must to collect to win the game */
	UPROPERTY(BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	int32 PickupsToCollect = 0;
	
	UPROPERTY(BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	FTimerHandle LevelTimer;

protected:

	/** flipflop the character's gravity  */
	void ChangeGravity();

	/** shows whether is  or not  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	uint32 bCanChangeGravity : 1;
	
};
