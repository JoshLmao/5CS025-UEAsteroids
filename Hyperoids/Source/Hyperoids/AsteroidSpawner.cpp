// Fill out your copyright notice in the Description page of Project Settings.


#include "AsteroidSpawner.h"

#include "Asteroids/BasicAsteroid.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet\GameplayStatics.h"

#include "Player\SpaceshipPawn.h"
#include "Player\SpaceshipPlayerState.h"
#include "HyperoidsGameModeBase.h"

AAsteroidSpawner::AAsteroidSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 2.0f;

	m_minimumActors = 5;
	m_actorsCount = 0;
	m_spawnArea = FVector2D(0.0f, 0.0f);
}

void AAsteroidSpawner::BeginPlay()
{
	Super::BeginPlay();

	m_player = (ASpaceshipPawn*)UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (!m_asteroid)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Asteroid set on AsteroidSpawner!"));
	}

	//DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FColor::Red, true, 1000, 0, 6);
}

void AAsteroidSpawner::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	SpawnAsteroids();
}

void AAsteroidSpawner::SpawnAsteroids()
{
	if (!m_asteroid || m_actorsCount > m_minimumActors)
		return;

	UWorld* world = GetWorld();
	if (!world)
		return;

	int amountToSpawn = FMath::RandRange(1, 3);
	for (int i = 0; i < amountToSpawn; i++) {

		// Spawn Asteroid and random location inside spawn area
		FVector2D randLocation = ABasicAsteroid::GetRndVectorInBoundary((FVector2D)GetComponentsBoundingBox().GetExtent(), m_spawnArea);
		FRotator randRotation = FRotator();

		// Convert random position to 3D and set
		FVector position;
		position.X = randLocation.X;
		position.Y = randLocation.Y;
		position.Z = 0.0f;

		// Spawn the asteroid
		ABasicAsteroid* asteroid = world->SpawnActor<ABasicAsteroid>(m_asteroid->GetDefaultObject()->GetClass(), position, randRotation);
		UE_LOG(LogTemp, Log, TEXT("Spawned asteroid at %s"), *position.ToString());

		// Give random spin & movement direction
		asteroid->SetRandomDirections();

		// Set a random scale to asteroid
		float minScale = 1.0f;
		float maxScale = 6.0f;
		FVector scale = FVector(FMath::RandRange(minScale, maxScale), FMath::RandRange(minScale, maxScale), FMath::RandRange(minScale, maxScale));
		asteroid->SetActorScale3D(scale);

		// Give the asteroids an amount to give to the player once destroyed
		asteroid->SetRewardScore(5);

		// Listen to event on asteroid that gets triggered when the asteroid gets destroyed
		asteroid->OnAsteroidDestroyed.AddDynamic(this, &AAsteroidSpawner::OnAsteroidDestroyed);
	}

	m_actorsCount += amountToSpawn;

	UE_LOG(LogTemp, Log, TEXT("Spawned '%d' asteroids"), amountToSpawn);
}

void AAsteroidSpawner::OnAsteroidDestroyed(ABasicAsteroid* asteroid)
{
	if (m_player && asteroid != nullptr)
	{
		ASpaceshipPlayerState* state = Cast<ASpaceshipPlayerState>(m_player->GetPlayerState());
		if (state)
			state->AddScore(asteroid->GetRewardScore());

		m_actorsCount -= 1;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to add score. No reference to Player!"));
	}
}

FVector2D AAsteroidSpawner::GetPlayArea()
{
	return (FVector2D)GetComponentsBoundingBox().GetExtent();
}
