// Fill out your copyright notice in the Description page of Project Settings.

#include "PlatformSwitcherKillBlock.h"
#include "ConstructorHelpers.h"

APlatformSwitcherKillBlock::APlatformSwitcherKillBlock()
{
	/**  set the default static mesh */
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Sphere(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (Sphere.Succeeded())
	{
		GetStaticMeshComponent()->SetStaticMesh(Sphere.Object);
	}

	GetStaticMeshComponent()->SetMobility(EComponentMobility::Static);
	GetStaticMeshComponent()->bGenerateOverlapEvents = true;
	GetStaticMeshComponent()->SetCollisionResponseToAllChannels(ECR_Block);
	GetStaticMeshComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}
