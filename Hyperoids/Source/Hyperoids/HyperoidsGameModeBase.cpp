// Fill out your copyright notice in the Description page of Project Settings.


#include "HyperoidsGameModeBase.h"

#include "SpaceshipPawn.h"

AHyperoidsGameModeBase::AHyperoidsGameModeBase()
{
	DefaultPawnClass = ASpaceshipPawn::StaticClass();
}