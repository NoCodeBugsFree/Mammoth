// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceInvadersMovementManager.h"
#include "SpaceInvadersEnemyBase.h"
#include "ConstructorHelpers.h"
#include "Components/ArrowComponent.h"
#include "AIController.h"
#include "Components/BoxComponent.h"
#include "SpaceInvadersEnemyBase.h"

// Sets default values
ASpaceInvadersMovementManager::ASpaceInvadersMovementManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	/* arrow component  */
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	SetRootComponent(ArrowComponent);

	/**  set the enemy class */
	static ConstructorHelpers::FClassFinder<ASpaceInvadersEnemyBase> EnemyTemplateClass(TEXT("/Game/BP/SpaceInvaders/BP_SpaceInvadersEnemy"));
	if (EnemyTemplateClass.Class)
	{
		EnemyClass = EnemyTemplateClass.Class;
	}

	/* left box collision  */
	LeftBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftBox"));
	LeftBox->SetupAttachment(RootComponent);
	LeftBox->SetBoxExtent(FVector(100.f, 100.f, 100.f));

	/* right box collision  */
	RightBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightBox"));
	RightBox->SetupAttachment(RootComponent);
	RightBox->SetBoxExtent(FVector(100.f, 100.f, 100.f));

	/* bottom box collision  */
	BottomBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BottomBox"));
	BottomBox->SetupAttachment(RootComponent);
	BottomBox->SetBoxExtent(FVector(100.f, 100.f, 100.f));
	
	/** defaults  */
	bActive = true;
}

// Called when the game starts or when spawned
void ASpaceInvadersMovementManager::BeginPlay()
{
	Super::BeginPlay();
	
	LeftBox->OnComponentBeginOverlap.AddDynamic(this, &ASpaceInvadersMovementManager::OnLeftBoxOverlapBegin);
	RightBox->OnComponentBeginOverlap.AddDynamic(this, &ASpaceInvadersMovementManager::OnRightBoxOverlapBegin);
	BottomBox->OnComponentBeginOverlap.AddDynamic(this, &ASpaceInvadersMovementManager::OnBottomBoxOverlapBegin);

	SpawnAllEnemies(ArrowComponent->GetComponentLocation());
	
	/** set fire timer  */
	FTimerHandle FIreTimer;
	GetWorldTimerManager().SetTimer(FIreTimer, this, &ASpaceInvadersMovementManager::FireEnemy, EnemyFireDelay, true);

	/** set fire timer  */
	FTimerHandle MoveTimer;
	GetWorldTimerManager().SetTimer(MoveTimer, this, &ASpaceInvadersMovementManager::MoveAllEnemies, EnemyMoveDelay, true);

	/** SetTimer */
	FTimerHandle DownTimer;
	GetWorldTimerManager().SetTimer(DownTimer, this, &ASpaceInvadersMovementManager::MoveDown, MoveDownDelay, true);
}

void ASpaceInvadersMovementManager::FireEnemy()
{
	int32 RandomIndex = FMath::RandRange(0, Enemies.Num() - 1);
	if (Enemies.IsValidIndex(RandomIndex))
	{
		Enemies[RandomIndex]->Fire();
	}
}

void ASpaceInvadersMovementManager::MoveAllEnemies()
{
	switch (MoveState)
	{
	case EMoveState::MS_Right:

		for (ASpaceInvadersEnemyBase* Enemy : Enemies)
		{
			if (Enemy)
			{
				Enemy->AddActorWorldOffset(FVector(0.f, GreedSize, 0.f), false, nullptr, ETeleportType::TeleportPhysics);
			}
		}
				
		break;
	
	case EMoveState::MS_Left:

		for (ASpaceInvadersEnemyBase* Enemy : Enemies)
		{
			if (Enemy)
			{
				Enemy->AddActorWorldOffset(FVector(0.f, -GreedSize, 0.f), false, nullptr, ETeleportType::TeleportPhysics);
			}
		}

		break;

	case EMoveState::MS_Down:

		for (ASpaceInvadersEnemyBase* Enemy : Enemies)
		{
			if (Enemy)
			{
				Enemy->AddActorWorldOffset(FVector(-GreedSize, 0.f, 0.f), false, nullptr, ETeleportType::TeleportPhysics);
			}
		}

		/** change moving state to left or right randomly */
		MoveState = FMath::RandBool() ? EMoveState::MS_Left : EMoveState::MS_Right;

		break;
	
	default:
		break;
				
	}
}

void ASpaceInvadersMovementManager::MoveDown()
{
	MoveState = EMoveState::MS_Down;
}

void ASpaceInvadersMovementManager::EnlistEnemy(class ASpaceInvadersEnemyBase* EnemyToAdd)
{
	Enemies.AddUnique(EnemyToAdd);
}

void ASpaceInvadersMovementManager::CheckOutEnemy(class ASpaceInvadersEnemyBase* EnemyToRemove)
{
	Enemies.Remove(EnemyToRemove);
	if (Enemies.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("WinTheGame"));
		WinTheGame();
	}
}

void ASpaceInvadersMovementManager::SpawnEnemy(FVector SpawnLocation)
{
	if (EnemyClass)
	{
		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator; //  GetController();
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			
			FRotator SpawnRotation = ArrowComponent->GetComponentRotation();
					
			ASpaceInvadersEnemyBase* Spawned = World->SpawnActor<ASpaceInvadersEnemyBase>(EnemyClass, SpawnLocation, SpawnRotation, SpawnParams);
			if (Spawned)
			{
				/** save enemy reference to array  */
				Enemies.AddUnique(Spawned);

				/** spawn controller  */
				if (Spawned->Controller == nullptr)
				{
					Spawned->SpawnDefaultController();
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("EnemyClass == NULL"));
	}
}

void ASpaceInvadersMovementManager::SpawnAllEnemies(FVector CenterLocation)
{
	for (int CurrentColumnIndex = 0; CurrentColumnIndex < Columns; CurrentColumnIndex++)
	{
		for (int CurrentRowIndex = 0; CurrentRowIndex < Rows; CurrentRowIndex++)
		{
			FVector Offset = FVector::ZeroVector;

			Offset.X = CurrentRowIndex - Rows / 2.f;

			Offset.Y = CurrentColumnIndex - Columns / 2.f;

			Offset *= GreedSize;

			FVector ToAdd = FVector(GreedSize, GreedSize, GreedSize) * FVector(0.5f, 0.5f, 0.f);

			Offset += ToAdd;
			
			Offset += CenterLocation;

			SpawnEnemy(Offset);
		}
	}
}

void ASpaceInvadersMovementManager::ChangeMovementDirection()
{
	if (bActive)
	{
		if (MoveState == EMoveState::MS_Right)
		{
			MoveState = EMoveState::MS_Left;
		}
		else
		{
			MoveState = EMoveState::MS_Right;
		}

		/** temporarily disable reverse of enemies movement  */
		bActive = false;

		/** activate back through 3 sec  */
		FTimerDelegate ResetDelegate;
		ResetDelegate.BindLambda([&] { bActive = true;  });
		
		FTimerHandle ResetTimer;
		GetWorldTimerManager().SetTimer(ResetTimer, ResetDelegate, 3.f, false);
	}
}

void ASpaceInvadersMovementManager::OnLeftBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// Other Actor is the actor that triggered the event. Check that is not ourself. 
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherActor->IsA(ASpaceInvadersEnemyBase::StaticClass()))
	{
		ChangeMovementDirection();
	}
}

void ASpaceInvadersMovementManager::OnRightBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// Other Actor is the actor that triggered the event. Check that is not ourself. 
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherActor->IsA(ASpaceInvadersEnemyBase::StaticClass()))
	{
		ChangeMovementDirection();
	}
}

void ASpaceInvadersMovementManager::OnBottomBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// Other Actor is the actor that triggered the event. Check that is not ourself. 
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherActor->IsA(ASpaceInvadersEnemyBase::StaticClass()))
	{
		if (bActive)
		{
			LoseTheGame();
			bActive = false;
		}
	}
}
