// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MoveToPickup.generated.h"

UCLASS()
class MAMMOTH_API AMoveToPickup : public AActor
{
	GENERATED_BODY()

	/** static mesh component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* SphereMesh;

	/** mesh dynamic material  */
	UPROPERTY(BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UMaterialInstanceDynamic* DynamicMaterial;
	
public:	

	// Sets default values for this actor's properties
	AMoveToPickup();

	virtual void OnConstruction(const FTransform& Transform) override;
};
