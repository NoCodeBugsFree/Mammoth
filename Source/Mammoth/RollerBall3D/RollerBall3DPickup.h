// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "RollerBall3DPickup.generated.h"

/**
 * 
 */
UCLASS()
class MAMMOTH_API ARollerBall3DPickup : public AStaticMeshActor
{
	GENERATED_BODY()
	
	/* material asset */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UMaterialInterface* PickupMaterial;

public:

	ARollerBall3DPickup();
	
	virtual void BeginPlay() override;
};
