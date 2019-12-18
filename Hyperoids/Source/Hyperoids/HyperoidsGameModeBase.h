// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Asteroids/BasicAsteroid.h"

#include "HyperoidsGameModeBase.generated.h"

/**
 * Game Mode for Hyperoids - Distributes the current play area
 */
UCLASS()
class HYPEROIDS_API AHyperoidsGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AHyperoidsGameModeBase();

	/** Gets the size of the current play area. FVector2D returns top-most & right-most vector*/
	FVector2D GetPlayArea();

private:
	virtual void StartPlay() override;

	/** Current area in which the player can play within */
	FVector2D m_playArea;
};
