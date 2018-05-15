// Fill out your copyright notice in the Description page of Project Settings.

#include "RollerBall3D_TPPickup.h"

ARollerBall3D_TPPickup::ARollerBall3D_TPPickup()
{
	GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
	GetStaticMeshComponent()->bGenerateOverlapEvents = true;
	GetStaticMeshComponent()->SetCollisionResponseToAllChannels(ECR_Block);
	GetStaticMeshComponent()->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
}
