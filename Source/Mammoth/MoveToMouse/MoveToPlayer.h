// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MoveToPlayer.generated.h"

UCLASS()
class MAMMOTH_API AMoveToPlayer : public ACharacter
{
	GENERATED_BODY()

	/** static mesh component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* SphereMesh;
	
public:

	AMoveToPlayer();

	virtual void Tick(float DeltaTime) override;
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
	
	virtual void BeginPlay() override;

	/** called to win the game */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "AAA")
	void WinTheGame();

	/** called to lose the game */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "AAA")
	void LoseTheGame();

	/** called whenever we pick up the pickup */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "AAA")
	void OnPickUp(bool bCorrect);
	
private:

	/** how far we should cast a ray  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float TraceDistance = 10000.f;

	/** how fast should we lerp movement to mouse location */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float InterpSpeed = 10.f;

	/** player controller reference  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class APlayerController* PC;

	/** Current Score  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	int32 Score = 0;

};
