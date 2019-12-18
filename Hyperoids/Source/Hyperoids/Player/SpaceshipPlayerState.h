// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SpaceshipPlayerState.generated.h"

/**
 * Overall state of the player containing their score
 */
UCLASS()
class HYPEROIDS_API ASpaceshipPlayerState : public APlayerState
{
	GENERATED_BODY()
		
	//! Current score of the player
	int Score;

public:
	ASpaceshipPlayerState();

	//! Gets the current score of the player
	int GetScore();
	//! Sets the current score of the player
	void SetScore(int score);

	//! Adds an amount to the current score of the player
	void AddScore(int amount);
	/** Removes an amount from the current players score, will not go below zero */
	void RemoveScore(int amount);
};
