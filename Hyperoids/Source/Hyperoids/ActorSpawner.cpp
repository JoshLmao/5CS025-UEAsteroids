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

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = SpawnTickInteval;
}

void AActorSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (!m_actorClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Actor set on %s!"), *GetName());
	}

	m_player = (ASpaceshipPawn*)UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void AActorSpawner::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

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
		FRotator randRotation = FRotator();

		// Add spawner global location to random location inside spawn volume
		FVector spawnerLocation = GetActorLocation();
		FVector position;
		position.X = spawnerLocation.X + randLocation.X;
		position.Y = spawnerLocation.Y + randLocation.Y;
		position.Z = spawnerLocation.Z;

		// Spawn the asteroid
		AActor* spawnedActor = world->SpawnActor<AActor>(m_actorClass->GetDefaultObject()->GetClass(), position, randRotation);

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
