// Fill out your copyright notice in the Description page of Project Settings.

#include "CrossShooterEnemy.h"
#include "ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "AI/Navigation/NavigationSystem.h"
#include "CrossShooterPlayer.h"

ACrossShooterEnemy::ACrossShooterEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/** mesh  */
	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	SphereMesh->SetupAttachment(RootComponent);
	SphereMesh->SetCollisionProfileName("OverlapOnlyPawn");

	/**  set the default static mesh */
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereTemplate(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (SphereTemplate.Succeeded())
	{
		SphereMesh->SetStaticMesh(SphereTemplate.Object);
	}

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ACrossShooterEnemy::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor && OtherActor != this && !OtherActor->IsPendingKill() && !IsPendingKill() && OtherActor->IsA(ACrossShooterPlayer::StaticClass()))
	{
		UGameplayStatics::ApplyDamage(OtherActor, 5.f, GetController(), this, UDamageType::StaticClass());
		SetActorEnableCollision(false);
		SetActorHiddenInGame(true);
		Destroy();
	}
}

void ACrossShooterEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	DynamicMaterial = SphereMesh->CreateDynamicMaterialInstance(0);
	
	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue("Color", FLinearColor::MakeRandomColor());
	}

	Player = UGameplayStatics::GetPlayerPawn(this, 0);
}

void ACrossShooterEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Player)
	{
		UNavigationSystem::SimpleMoveToActor(GetController(), Player);
	}
}
