// Fill out your copyright notice in the Description page of Project Settings.

#include "MoveToPickup.h"
#include "ConstructorHelpers.h"

// Sets default values
AMoveToPickup::AMoveToPickup()
{
	/** mesh  */
	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	SetRootComponent(SphereMesh);
	SphereMesh->SetCollisionResponseToAllChannels(ECR_Block);
	SphereMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereMesh->SetMobility(EComponentMobility::Static);

	/**  set the default static mesh */
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Sphere(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (Sphere.Succeeded())
	{
		SphereMesh->SetStaticMesh(Sphere.Object);
	}
}

void AMoveToPickup::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	DynamicMaterial = SphereMesh->CreateDynamicMaterialInstance(0);

	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue("Color", FLinearColor::MakeRandomColor());
		DynamicMaterial->SetScalarParameterValue("Metallic", FMath::RandRange(0.f, 1.f));
		DynamicMaterial->SetScalarParameterValue("Roughness", FMath::RandRange(0.f, 1.f));
	}
}

