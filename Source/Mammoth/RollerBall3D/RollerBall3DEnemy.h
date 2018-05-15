// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RollerBall3D/RollerBall3DPlayer.h"
#include "RollerBall3DEnemy.generated.h"

/**
 * 
 */
UCLASS()
class MAMMOTH_API ARollerBall3DEnemy : public ARollerBall3DPlayer
{
	GENERATED_BODY()
	
public:


	
protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	/** called to set new target point  */
	void SetDestination();

	// -----------------------------------------------------------------------------------

	/* radius */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float Radius = 5000.f;
	
};
