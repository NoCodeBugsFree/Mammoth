// Fill out your copyright notice in the Description page of Project Settings.

#include "RollerBall3DPickup.h"
#include "ConstructorHelpers.h"
#include "Materials/MaterialInterface.h"

ARollerBall3DPickup::ARollerBall3DPickup()
{
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
}

void ARollerBall3DPickup::BeginPlay()
{
	Super::BeginPlay();

	UMaterialInstanceDynamic*  DynamicMaterial = GetStaticMeshComponent()->CreateDynamicMaterialInstance(0);
	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue("Color", FLinearColor::MakeRandomColor());
	}
}
