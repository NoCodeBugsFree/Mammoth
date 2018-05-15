// Fill out your copyright notice in the Description page of Project Settings.

#include "GravityPlatformerObstacle.h"
#include "ConstructorHelpers.h"
#include "Materials/MaterialInterface.h"

AGravityPlatformerObstacle::AGravityPlatformerObstacle()
{
	PrimaryActorTick.bCanEverTick = false;

	GetStaticMeshComponent()->bGenerateOverlapEvents = true;
	GetStaticMeshComponent()->SetCollisionProfileName("OverlapOnlyPawn");
	
	/**  set the default static mesh */
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeTemplate(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (CubeTemplate.Succeeded())
	{
		GetStaticMeshComponent()->SetStaticMesh(CubeTemplate.Object);
	}
	
	/**  set material */
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> DynamicMaterialAsset(TEXT("/Game/BP/MoveToMouse/M_Dynamic"));
	if (DynamicMaterialAsset.Succeeded())
	{
		CubeMaterial = DynamicMaterialAsset.Object;
		if (CubeMaterial)
		{
			GetStaticMeshComponent()->SetMaterial(0, CubeMaterial);
		}
	}
}

void AGravityPlatformerObstacle::BeginPlay()
{
	Super::BeginPlay();

	DynamicMaterial = GetStaticMeshComponent()->CreateDynamicMaterialInstance(0);

	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue("Color", FLinearColor::MakeRandomColor());
	}
}