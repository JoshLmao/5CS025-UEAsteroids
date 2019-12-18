// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "ActorSpawner.generated.h"

/**
 * Generic actor spawner for creating actors inside the area of the trigger box
 */
UCLASS()
class HYPEROIDS_API AActorSpawner : public ATriggerBox
{
	GENERATED_BODY()
public:
	AActorSpawner();

	// Inteval at which an actor will spawn inside the spawn volume
	UPROPERTY(EditAnywhere)
	float SpawnTickInteval;

	virtual void Tick(float deltaTime) override;

	//! Gets the current area of the actor spawner
	FVector2D GetSpawnerArea();

protected:
	/** Type of actor to spawn inside the spawn volume */
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> m_actorClass;

	/** Safe area inside the spawn volume that won't spawn any actors */
	UPROPERTY(EditAnywhere)
	FVector2D m_spawnArea;

	/** Total spawn area where asteroids can spawn */
	UPROPERTY()
	class UBoxComponent* m_spawnVolume;

	/** The minimum amount of asteroids that will be spawned at once inside the area */
	UPROPERTY(EditAnywhere)
	int m_minimumActors;

	/** Amount of asteroid actors that are already spawned */
	int m_actorsCount;

	/** Should the spawner run the first tick when the game has just begun to play */
	UPROPERTY(EditAnywhere)
	bool m_bShouldTickOnStart;

	/** Reference to the player for inherited classes to use */
	class ASpaceshipPawn* m_player;

	virtual void BeginPlay() override;

	//! Called once an actor has been spawned inside the actor spawner
	virtual void OnSpawnActor(AActor* actor);

private:
	//! Has the spawner run it's first tick yet
	bool m_bIsFirstTick;

	//! Spawns actors with the current class properties
	void SpawnActors();
};
