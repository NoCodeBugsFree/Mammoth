// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceShooterGameMode.h"
#include "SpaceShooterHUD.h"
#include "SpaceShooterPlayer.h"
#include "Board.h"

ASpaceShooterGameMode::ASpaceShooterGameMode()
{
	HUDClass = ASpaceShooterHUD::StaticClass();
	DefaultPawnClass = ASpaceShooterPlayer::StaticClass();
}