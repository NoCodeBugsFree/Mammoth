// Fill out your copyright notice in the Description page of Project Settings.

#include "CatchingGamePickup.h"
#include "ConstructorHelpers.h"
#include "Materials/MaterialInterface.h"

ACatchingGamePickup::ACatchingGamePickup()
{
	PrimaryActorTick.bCanEverTick = true;
	
	/**  set mesh asset */
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Sphere(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (Sphere.Succeeded())
	{
		GetStaticMeshComponent()->SetStaticMesh(Sphere.Object);
	}

	/**  set material */
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialTemplate(TEXT("/Game/BP/MoveToMouse/M_Dynamic_Inst"));
	if (MaterialTemplate.Succeeded())
	{
		PickupMaterial = MaterialTemplate.Object;
		if (PickupMaterial)
		{
			GetStaticMeshComponent()->SetMaterial(0, PickupMaterial);
		}
	}

	GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
	GetStaticMeshComponent()->SetSimulatePhysics(true);
	GetStaticMeshComponent()->bGenerateOverlapEvents = true;

	InitialLifeSpan = 10.f;
}

void ACatchingGamePickup::BeginPlay()
{
	Super::BeginPlay();
	
	UMaterialInstanceDynamic*  DynamicMaterial = GetStaticMeshComponent()->CreateDynamicMaterialInstance(0);
	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue("Color", FLinearColor::MakeRandomColor());
	}
}

void ACatchingGamePickup::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	FVector CurrentScale3D = GetActorScale3D();

	/** deflate the balloon  */
	SetActorScale3D(FMath::VInterpTo(CurrentScale3D, DestroyActorScale3D, DeltaSeconds, InterpSpeed));

	/** destroy the actor if it is small enough  */
	if (CurrentScale3D.Equals(DestroyActorScale3D, 0.1f))
	{
		SetActorTickEnabled(false);
		Destroy();
	}
}
