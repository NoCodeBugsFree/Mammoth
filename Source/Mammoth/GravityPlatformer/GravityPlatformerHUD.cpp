// Fill out your copyright notice in the Description page of Project Settings.

#include "GravityPlatformerHUD.h"
#include "Components/TextBlock.h"
#include "GravityPlatformerPlayer.h"

bool UGravityPlatformerHUD::Initialize()
{
	Super::Initialize();

	if(AGravityPlatformerPlayer* Player = Cast<AGravityPlatformerPlayer>(GetOwningPlayerPawn()))
	{
		Owner = Player;
	}

	return true;
}

void UGravityPlatformerHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (Owner)
	{
		FString ScoreString = FString::Printf(TEXT("Score: %d"), Owner->Score);
		FText ScoreText = FText::FromString(ScoreString);
		Score->SetText(ScoreText);

		FString TimeString = FString::Printf(TEXT("Time: %d"), (int32)GetWorld()->GetTimerManager().GetTimerRemaining(Owner->LevelTimer));
		FText TimeText = FText::FromString(TimeString);
		Time->SetText(TimeText);
	}
}
