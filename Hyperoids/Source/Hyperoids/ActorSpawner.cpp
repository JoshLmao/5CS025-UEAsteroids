// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorSpawner.h"

#include "Asteroids/BasicAsteroid.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet\GameplayStatics.h"

#include "Player\SpaceshipPawn.h"
#include "Player\SpaceshipPlayerState.h"
#include "HyperoidsGameModeBase.h"

AActorSpawner::AActorSpawner()
{
	SpawnTickInteval = 2.0f;

	m_player = nullptr;
	m_minimumActors = 5;
	m_actorsCount = 0;
	m_spawnArea = FVector2D(0.0f, 0.0f);
	m_bIsFirstTick = true;
	m_bShouldTickOnStart = true;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = SpawnTickInteval;
}

void AActorSpawner::BeginPlay()
{
	Super::BeginPlay();

	PrimaryActorTick.TickInterval = SpawnTickInteval;

	if (!m_actorClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Actor set on '%s'!"), *GetName());
	}

	m_player = (ASpaceshipPawn*)UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void AActorSpawner::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
	UE_LOG(LogTemp, Warning, TEXT("STARTING SPAWNING ACTORS"));

	if (!m_bShouldTickOnStart && m_bIsFirstTick) {
		m_bIsFirstTick = false;
		return;
	}

	SpawnActors();
}

void AActorSpawner::SpawnActors()
{
	if (!m_actorClass || m_actorsCount > m_minimumActors)
		return;

	UWorld* world = GetWorld();
	if (!world)
		return;

	int amountToSpawn = 1;
	for (int i = 0; i < amountToSpawn; i++) {

		// Spawn Asteroid and random location inside spawn area
		FVector2D randLocation = ABasicAsteroid::GetRndVectorInBoundary(GetSpawnerArea(), m_spawnArea);
		FRotator rotation = FRotator(0.0f, 0.0f, 0.0f);

		// Add spawner global location to random location inside spawn volume
		FVector spawnerLocation = GetActorLocation();
		FVector position;
		position.X = randLocation.X;
		position.Y = randLocation.Y;
		position.Z = spawnerLocation.Z;

		position.X += spawnerLocation.X;
		position.Y += spawnerLocation.Y;

		// Spawn the asteroid
		AActor* spawnedActor = world->SpawnActor<AActor>(m_actorClass->GetDefaultObject()->GetClass(), position, rotation);

		OnSpawnActor(spawnedActor);
	}

	m_actorsCount += amountToSpawn;
	UE_LOG(LogTemp, Log, TEXT("Spawned '%d' '%s' actor(s)"), amountToSpawn, *m_actorClass.Get()->GetName());
}

void AActorSpawner::OnSpawnActor(AActor* actor)
{
	// Can be overridden to configure the actor
}

FVector2D AActorSpawner::GetSpawnerArea()
{
	return (FVector2D)GetComponentsBoundingBox().GetExtent();
}
