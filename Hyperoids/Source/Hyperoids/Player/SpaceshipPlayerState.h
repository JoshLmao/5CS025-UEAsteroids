// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SpaceshipPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class HYPEROIDS_API ASpaceshipPlayerState : public APlayerState
{
	GENERATED_BODY()
		
	int Score;

public:
	ASpaceshipPlayerState();

	int GetScore();
	void SetScore(int score);

	void AddScore(int amount);
	void RemoveScore(int amount);
};
