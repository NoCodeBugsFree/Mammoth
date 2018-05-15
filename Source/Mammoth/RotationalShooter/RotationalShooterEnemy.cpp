// Fill out your copyright notice in the Description page of Project Settings.

#include "RotationalShooterEnemy.h"
#include "ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "AI/Navigation/NavigationSystem.h"
#include "RotationalShooterPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

ARotationalShooterEnemy::ARotationalShooterEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->SetEnableGravity(false);
	
	/** mesh  */
	TestSphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cone"));
	TestSphereMesh->SetupAttachment(RootComponent);
	TestSphereMesh->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	TestSphereMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TestSphereMesh->bGenerateOverlapEvents = false;
	TestSphereMesh->BodyInstance.bNotifyRigidBodyCollision = false;

	/**  set the default static mesh */
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereTemplate(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (SphereTemplate.Succeeded())
	{
		TestSphereMesh->SetStaticMesh(SphereTemplate.Object);
	}

	/* sphere collision  */
	KillSphere = CreateDefaultSubobject<USphereComponent>(TEXT("KillSphere"));
	KillSphere->SetupAttachment(RootComponent);
	KillSphere->SetSphereRadius(55.f);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ARotationalShooterEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	TargetPawn = UGameplayStatics::GetPlayerPawn(this, 0);

	DynamicMaterial = TestSphereMesh->CreateDynamicMaterialInstance(0);
	
	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue("Color", FLinearColor::MakeRandomColor());
	}

	KillSphere->OnComponentBeginOverlap.AddDynamic(this, &ARotationalShooterEnemy::OnKillSphereOverlapBegin);
}

void ARotationalShooterEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetPawn)
	{
		UNavigationSystem::SimpleMoveToActor(GetController(), TargetPawn);
	}
}

void ARotationalShooterEnemy::OnKillSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// Other Actor is the actor that triggered the event. Check that is not ourself. 
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if(ARotationalShooterPlayer* Player = Cast<ARotationalShooterPlayer>(OtherActor))
		{
			UGameplayStatics::ApplyDamage(Player, 10.f, GetController(), this, UDamageType::StaticClass());
			SetActorEnableCollision(false);
			SetActorHiddenInGame(true);
			Destroy();
		}
	}
}
