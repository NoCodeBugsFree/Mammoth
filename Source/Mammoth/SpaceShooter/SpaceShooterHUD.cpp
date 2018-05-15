// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceShooterHUD.h"
#include "SpaceShooterPlayer.h"
#include "ConstructorHelpers.h"
#include "SpaceShooterHUDWidget.h"
#include "Components/TextBlock.h"

ASpaceShooterHUD::ASpaceShooterHUD()
{
	/**  set the widget asset */
	static ConstructorHelpers::FClassFinder<UUserWidget> HUDWidgetClassBPClass(TEXT("/Game/BP/SpaceShooter/WBP_SpaceShooterHUD"));
	if (HUDWidgetClassBPClass.Class)
	{
		HUDWidgetClass = HUDWidgetClassBPClass.Class;
	}
}

void ASpaceShooterHUD::BeginPlay()
{
	Super::BeginPlay();

	/** set owner ref  */
	if (ASpaceShooterPlayer* TestSpaceShooterPlayer = Cast<ASpaceShooterPlayer>(GetOwningPawn()))
	{
		SpaceShooterPlayer = TestSpaceShooterPlayer;
	}

	/** create widget and add it to viewport  */
	if (HUDWidgetClass)
	{
		CurrentWidget = CreateWidget<USpaceShooterHUDWidget>(GetWorld(), HUDWidgetClass);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
}

void ASpaceShooterHUD::DrawHUD()
{
	Super::DrawHUD();

	if (SpaceShooterPlayer && CurrentWidget)
	{
		FString ScoreText = FString::Printf(TEXT("Score: %d"), SpaceShooterPlayer->GetScore());
		FString HealthText = FString::Printf(TEXT("Health: %d"), (int32)SpaceShooterPlayer->GetHealth());
		
		CurrentWidget->Score->SetText(FText::FromString(ScoreText));
		CurrentWidget->Health->SetText(FText::FromString(HealthText));
	}
}
