// Fill out your copyright notice in the Description page of Project Settings.


#include "HyperoidsGameModeBase.h"

#include "Player/SpaceshipPawn.h"
#include "Engine/World.h"
#include "Engine/Public/TimerManager.h"
#include "Kismet/GameplayStatics.h"

#include "UI/HyperoidsHUD.h"
#include "Player/SpaceshipPlayerState.h"

const int AHyperoidsGameModeBase::MAX_ASTEROIDS = 8;

AHyperoidsGameModeBase::AHyperoidsGameModeBase()
{
	// Set default classes for game objects
	DefaultPawnClass = ASpaceshipPawn::StaticClass();
	PlayerStateClass = ASpaceshipPlayerState::StaticClass();
	HUDClass = AHyperoidsHUD::StaticClass();

	m_playArea = FVector2D(1000.0f, 1900.0f);
	m_spawnArea = FVector2D(300.0f, 300.0f);
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

	m_player = (ASpaceshipPawn*)UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

FVector2D AHyperoidsGameModeBase::GetPlayArea()
{
	return m_playArea;
}

FVector2D AHyperoidsGameModeBase::GetSpawnArea()
{
	return m_spawnArea;
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
		const FVector randLocation = ABasicAsteroid::GetRndVectorInBoundary(m_playArea);
		const FRotator randRotation = FRotator();
		ABasicAsteroid* asteroid = world->SpawnActor<ABasicAsteroid>(ABasicAsteroid::StaticClass(), randLocation, randRotation);
		// Give random spin & movement direction
		asteroid->SetRandomDirections();

		// Give a random scale to asteroid
		float minScale = 1.0f;
		float maxScale = 6.0f;
		FVector scale = FVector(FMath::RandRange(minScale, maxScale), FMath::RandRange(minScale, maxScale), FMath::RandRange(minScale, maxScale));
		asteroid->SetActorScale3D(scale);

		asteroid->SetRewardScore(5);

		m_asteroids.Add(asteroid);
	}

	UE_LOG(LogTemp, Log, TEXT("Spawned '%d' asteroids"), amountToSpawn)
}

void AHyperoidsGameModeBase::OnAsteroidDestroyed(ABasicAsteroid* asteroid)
{
	if (m_player)
	{
		ASpaceshipPlayerState* state = Cast<ASpaceshipPlayerState>(m_player->GetPlayerState());
		if (state) 
		{
			state->AddScore(asteroid->GetRewardScore());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to add score. No reference to Player!"));
	}
}

void AHyperoidsGameModeBase::PlayerDeath(AActor* player)
{
	OnPlayerDeath.Broadcast(player);
}