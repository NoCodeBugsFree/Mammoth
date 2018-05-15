// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpaceInvadersMovementManager.generated.h"

/** describes current enemies movement state  */
UENUM(BlueprintType)
enum class EMoveState : uint8
{
	MS_Right	UMETA(DisplayName = "Right"),
	MS_Left		UMETA(DisplayName = "Left"),
	MS_Down		UMETA(DisplayName = "Down")
};

UCLASS()
class MAMMOTH_API ASpaceInvadersMovementManager : public AActor
{
	GENERATED_BODY()

	/* arrow component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* ArrowComponent;

	/* left box collision  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* LeftBox;
	
	/* right box collision  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* RightBox;
	
	/* bottom box collision  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BottomBox;
	
public:	
	// Sets default values for this actor's properties
	ASpaceInvadersMovementManager();

	/** called to add the enemy to the Enemies array  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void EnlistEnemy(class ASpaceInvadersEnemyBase* EnemyToAdd);

	/** called to remove the enemy from the Enemies array  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void CheckOutEnemy(class ASpaceInvadersEnemyBase* EnemyToRemove);

	/** called to spawn all enemies */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void SpawnAllEnemies(FVector CenterLocation);

	/** called set new move state */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void SetMoveState(EMoveState NewMoveState) { MoveState = NewMoveState; }
	
protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** called whenever enemies reach bottom box */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "AAA")
	void LoseTheGame();

	/** called whenever all enemies are killed */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "AAA")
	void WinTheGame();

private:

	/** shows whether right and left boxes are active or not  */
	uint32 bActive : 1;
	
	/** called to enforce all enemies go opposite direction  */
	void ChangeMovementDirection();

	/** called to change movement direction of all the enemies */
	UFUNCTION()
	void OnLeftBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	/** called to change movement direction of all the enemies */
	UFUNCTION()
	void OnRightBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	/** called to lose the game */
	UFUNCTION()
	void OnBottomBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	/** called to one enemy at specified location */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void SpawnEnemy(FVector SpawnLocation);

	UFUNCTION(BlueprintCallable, Category = "AAA")
	void FireEnemy();

	UFUNCTION(BlueprintCallable, Category = "AAA")
	void MoveAllEnemies();

	UFUNCTION(BlueprintCallable, Category = "AAA")
	void MoveDown();

	// -----------------------------------------------------------------------------------

	/** current enemies move state  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (AllowPrivateAccess = "true"))
	EMoveState MoveState;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ASpaceInvadersEnemyBase> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	int32 Rows = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	int32 Columns = 5;

	/** distance between enemies  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float GreedSize = 150.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float EnemyFireDelay = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float EnemyMoveDelay = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float MoveDownDelay = 7.f;
	
	/** all enemies  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	TArray<class ASpaceInvadersEnemyBase*> Enemies;
	
};
