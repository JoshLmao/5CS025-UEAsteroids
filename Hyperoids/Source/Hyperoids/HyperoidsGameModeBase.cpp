// Fill out your copyright notice in the Description page of Project Settings.


#include "HyperoidsGameModeBase.h"

#include "Player/SpaceshipPawn.h"
#include "Engine/World.h"
#include "Engine/Public/TimerManager.h"

const int AHyperoidsGameModeBase::MAX_ASTEROIDS = 15;

AHyperoidsGameModeBase::AHyperoidsGameModeBase()
{
	DefaultPawnClass = ASpaceshipPawn::StaticClass();

	m_playArea = FVector2D(1000.0f, 1900.0f);
}

void AHyperoidsGameModeBase::StartPlay()
{
	Super::StartPlay();

	// Spawn Asteroids on first launch
	SpawnAsteroids();

	// SetTimer to trigger spawning new asteroids every X seconds
	if (!SpawnHandle_CreateAsteroids.IsValid())
	{
		float delaySeconds = 15.0f;
		GetWorldTimerManager().SetTimer(SpawnHandle_CreateAsteroids, this, &AHyperoidsGameModeBase::SpawnAsteroids, delaySeconds, true);
	}
}

FVector2D AHyperoidsGameModeBase::GetPlayArea()
{
	return m_playArea;
}

void AHyperoidsGameModeBase::SpawnAsteroids()
{
	if (m_asteroids.Num() >= MAX_ASTEROIDS)
	{
		// Don't spawn any more asteroids until others are destroyed
		return;
	}

	int amountToSpawn = FMath::RandRange(3, 6);
	UWorld* world = GetWorld();
	for (int i = 0; i < amountToSpawn; i++) 
	{
		const FVector randLocation = FVector();
		const FRotator randRotation = FRotator();
		ABasicAsteroid* asteroid = world->SpawnActor<ABasicAsteroid>(ABasicAsteroid::StaticClass(), randLocation, randRotation);
		asteroid->SetRandomDirections();

		//Give random location on initial spawn
		asteroid->SetRandomLocation();

		// Give a random scale to asteroid
		float minScale = 1.0f;
		float maxScale = 6.0f;
		FVector scale = FVector(FMath::RandRange(minScale, maxScale), FMath::RandRange(minScale, maxScale), FMath::RandRange(minScale, maxScale));
		asteroid->SetActorScale3D(scale);

		m_asteroids.Add(asteroid);
	}

	UE_LOG(LogTemp, Log, TEXT("Spawned '%d' asteroids"), amountToSpawn)
}