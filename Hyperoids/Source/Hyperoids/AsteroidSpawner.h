// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "AsteroidSpawner.generated.h"

/**
 * 
 */
UCLASS()
class HYPEROIDS_API AAsteroidSpawner : public ATriggerBox
{
	GENERATED_BODY()
	
public:
	AAsteroidSpawner();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float deltaTime) override;
	
	UFUNCTION()
	void OnAsteroidDestroyed(class ABasicAsteroid* asteroid);

	FVector2D GetPlayArea();

private:
	/* Type of asteroid to spawn inside the spawn volume */
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABasicAsteroid> m_asteroid;

	/* Safe area inside the spawn volume that won't spawn any asteroids*/
	UPROPERTY(EditAnywhere)
	FVector2D m_spawnArea;

	/* Total spawn area where asteroids can spawn */
	UPROPERTY()
	class UBoxComponent* m_spawnVolume;

	/* The minimum amount of asteroids that will be spawned at once inside the area */
	UPROPERTY(EditAnywhere)
	int m_minimumActors;

	/* Amount of asteroid actors that are already spawned */
	int m_actorsCount;

	class ASpaceshipPawn* m_player;

	void SpawnAsteroids();
};
