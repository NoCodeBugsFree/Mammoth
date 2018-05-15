// Fill out your copyright notice in the Description page of Project Settings.

#include "Board.h"
#include "Components/BoxComponent.h"
#include "ConstructorHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "SpaceShooterEnemy.h"
#include "Components/ArrowComponent.h"

// Sets default values
ABoard::ABoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	/* scene component  */
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	/* box collision  */
	EnemySpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	EnemySpawnBox->SetupAttachment(RootComponent);
	EnemySpawnBox->SetBoxExtent(FVector(100.f, 100.f, 100.f));

	/**  set the  class */
	static ConstructorHelpers::FClassFinder<ASpaceShooterEnemy> EnemyTemplateClass(TEXT("/Game/BP/SpaceShooter/BP_SpaceShooterEnemy"));
	if (EnemyTemplateClass.Class)
	{
		EnemyClass = EnemyTemplateClass.Class;
	}

	/* arrow component  */
	SpawnRotaion = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	SpawnRotaion->SetupAttachment(RootComponent);
}

void ABoard::SpawnEnemy()
{
	if (EnemyClass)
	{
		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			
			FVector SpawnOrigin = EnemySpawnBox->Bounds.Origin;
			FVector SpawnExtent = EnemySpawnBox->Bounds.BoxExtent;
			
			FVector SpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(SpawnOrigin, SpawnExtent);
			SpawnLocation.Z = 100.f;
			FRotator SpawnRotation = SpawnRotaion->GetComponentRotation();
			
			ASpaceShooterEnemy* Spawned = World->SpawnActor<ASpaceShooterEnemy>(EnemyClass, SpawnLocation, SpawnRotation, SpawnParams);
			if (Spawned)
			{
				
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("WhatToSpawnClass == NULL"));
	}
}

// Called when the game starts or when spawned
void ABoard::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle SpawnTimer;
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &ABoard::SpawnEnemy, 0.1f, true);
}