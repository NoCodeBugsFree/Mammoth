// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SpaceShooterHUD.generated.h"

/**
 * 
 */
UCLASS()
class MAMMOTH_API ASpaceShooterHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	ASpaceShooterHUD();

	virtual void BeginPlay() override;

	virtual void DrawHUD() override;
	
private:

	/** widget class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class USpaceShooterHUDWidget> HUDWidgetClass;


	/** current active widget */
	UPROPERTY()
	class USpaceShooterHUDWidget* CurrentWidget;
	
	UPROPERTY(BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class ASpaceShooterPlayer* SpaceShooterPlayer;
};
