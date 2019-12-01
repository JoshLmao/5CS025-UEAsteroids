// Fill out your copyright notice in the Description page of Project Settings.


#include "HyperoidsGameModeBase.h"

#include "Player/SpaceshipPawn.h"
#include "Engine/World.h"

AHyperoidsGameModeBase::AHyperoidsGameModeBase()
{
	DefaultPawnClass = ASpaceshipPawn::StaticClass();

	m_playArea = FVector2D(1000.0f, 1900.0f);
}

void AHyperoidsGameModeBase::StartPlay()
{
	Super::StartPlay();

	UWorld* world = GetWorld();
	for (int i = 0; i < 4; i++) {
		UClass* basicAsteroid = ABasicAsteroid::StaticClass();

		FActorSpawnParameters params;
		params.Name = TEXT("Asteroid " + i);
		AActor* asteroid = world->SpawnActor(basicAsteroid, NULL, params);
		
		m_asteroids.Add(asteroid);
	}
}

FVector2D AHyperoidsGameModeBase::GetPlayArea()
{
	return m_playArea;
}
