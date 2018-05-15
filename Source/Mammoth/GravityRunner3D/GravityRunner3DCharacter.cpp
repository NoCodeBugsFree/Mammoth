// Fill out your copyright notice in the Description page of Project Settings.

#include "GravityRunner3DCharacter.h"
#include "ConstructorHelpers.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AGravityRunner3DCharacter::AGravityRunner3DCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/** mesh  */
	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ConeMesh"));
	SphereMesh->SetupAttachment(RootComponent);
	SphereMesh->bGenerateOverlapEvents = false;
	SphereMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	/**  set the default static mesh */
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereTemplate(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (SphereTemplate.Succeeded())
	{
		SphereMesh->SetStaticMesh(SphereTemplate.Object);
	}

	/** camera boom  */
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 2000.f;
	CameraBoom->SetRelativeRotation(FRotator(-10.f, -90.f, 0.f));
	CameraBoom->bAbsoluteRotation = true;
	CameraBoom->bDoCollisionTest = false;

	/* camera component  */
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraBoom);

	/** setup capsule  */
	GetCapsuleComponent()->SetCapsuleHalfHeight(50.f);
	GetCapsuleComponent()->SetCapsuleRadius(50.f);

	/** sensitive air control  */
	GetCharacterMovement()->AirControl = 1.f;

	/** simulate ceiling movement as floor as well  */
	GetCharacterMovement()->bUseSeparateBrakingFriction = true;
	GetCharacterMovement()->BrakingFriction = 10.f;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;

}

// Called when the game starts or when spawned
void AGravityRunner3DCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGravityRunner3DCharacter::ChangeGravity()
{
	/** reverse gravity  */
	GetCharacterMovement()->GravityScale *= -1.f;

	/** I want to fly  */
	Jump();
}

// Called every frame
void AGravityRunner3DCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/** constantly move forward  */
	AddMovementInput(GetActorForwardVector());
}

// Called to bind functionality to input
void AGravityRunner3DCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/** gravity  */
	PlayerInputComponent->BindAction("ChangeGravity", IE_Pressed, this, &AGravityRunner3DCharacter::ChangeGravity);
}

void AGravityRunner3DCharacter::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);


}

