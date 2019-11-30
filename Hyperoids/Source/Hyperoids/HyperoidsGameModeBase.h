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

private:
	TArray<AActor*> m_asteroids;

	virtual void StartPlay() override;
};