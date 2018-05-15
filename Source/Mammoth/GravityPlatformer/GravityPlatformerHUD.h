// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GravityPlatformerHUD.generated.h"

/**
 * 
 */
UCLASS()
class MAMMOTH_API UGravityPlatformerHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(meta = (BindWidget), VisibleAnywhere, BlueprintReadWrite)
	class UTextBlock* Score;

	UPROPERTY(meta = (BindWidget), VisibleAnywhere, BlueprintReadWrite)
	class UTextBlock* Time;
	
	virtual bool Initialize() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

private:

	/** owner player reference  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class AGravityPlatformerPlayer* Owner;

};
