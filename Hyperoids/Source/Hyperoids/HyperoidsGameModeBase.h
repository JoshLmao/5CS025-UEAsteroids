// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Asteroids/BasicAsteroid.h"

#include "HyperoidsGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class HYPEROIDS_API AHyperoidsGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AHyperoidsGameModeBase();

	/* Gets the size of the current play area. FVector2D returns top-most & right-most vector*/
	FVector2D GetPlayArea();

	static const int MAX_ASTEROIDS;

private:
	TArray<AActor*> m_asteroids;
	FVector2D m_playArea;

	FTimerHandle SpawnHandle_CreateAsteroids;

	virtual void StartPlay() override;

	UFUNCTION()
	void SpawnAsteroids();

	FVector GetRndVectorInBoundary(float maxX, float maxY);
};
