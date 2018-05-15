// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "GravityPlatformerObstacle.generated.h"

/**
 * 
 */
UCLASS()
class MAMMOTH_API AGravityPlatformerObstacle : public AStaticMeshActor
{
	GENERATED_BODY()

	/* cube material */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UMaterialInterface* CubeMaterial;

	/** DMI reference  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UMaterialInstanceDynamic* DynamicMaterial;
	
public:

	AGravityPlatformerObstacle();

	virtual void BeginPlay() override;
	
	
};
