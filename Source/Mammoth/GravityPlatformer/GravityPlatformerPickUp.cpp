// Fill out your copyright notice in the Description page of Project Settings.

#include "GravityPlatformerPickUp.h"
#include "ConstructorHelpers.h"

AGravityPlatformerPickUp::AGravityPlatformerPickUp()
{
	/**  set the default static mesh */
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeTemplate(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (CubeTemplate.Succeeded())
	{
		GetStaticMeshComponent()->SetStaticMesh(CubeTemplate.Object);
	}
}
