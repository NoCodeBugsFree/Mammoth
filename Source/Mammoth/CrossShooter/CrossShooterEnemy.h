// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CrossShooterEnemy.generated.h"

UCLASS()
class MAMMOTH_API ACrossShooterEnemy : public ACharacter
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* SphereMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UMaterialInstanceDynamic* DynamicMaterial;

public:
	
	ACrossShooterEnemy();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
protected:
	
	virtual void BeginPlay() override;

private:
	
	UPROPERTY()
	class APawn* Player;

public:	
	
	virtual void Tick(float DeltaTime) override;
	
};
