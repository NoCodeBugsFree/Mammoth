// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpaceShooterHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class MAMMOTH_API USpaceShooterHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(meta = (BindWidget), VisibleAnywhere, BlueprintReadWrite)
	class UTextBlock* Score;

	UPROPERTY(meta = (BindWidget), VisibleAnywhere, BlueprintReadWrite)
	class UTextBlock* Health;
	
};
