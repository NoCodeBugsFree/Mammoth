// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "CatchingGamePickup.generated.h"

/**
 * 
 */
UCLASS()
class MAMMOTH_API ACatchingGamePickup : public AStaticMeshActor
{
	GENERATED_BODY()

	/* material asset */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UMaterialInterface* PickupMaterial;
	
public:

	ACatchingGamePickup();

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;

private:

	/** percent deflate per sec from initial scale  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float DeflateSpeed = 30.f;

	/** desired scale amount at which we will destroy this actor  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	FVector DestroyActorScale3D = FVector(0.1f, 0.1f, 0.1f);

	/**  how fast we should deflate  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float InterpSpeed = 0.5f;
};
