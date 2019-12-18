// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorSpawner.h"
#include "AlienShipSpawner.generated.h"

/**
 * Spawner to create enemy alien ships that can fire at the player
 */
UCLASS()
class HYPEROIDS_API AAlienShipSpawner : public AActorSpawner
{
	GENERATED_BODY()

public:
	//! Sets default values for this actor's properties
	AAlienShipSpawner();

protected:
	virtual void OnSpawnActor(AActor* actor) override;

private:
	/** Method for listening to when the AlienShip has died */
	UFUNCTION()
	void OnAlienShipDeath(class AAlienShip* alienShip);

	//! Minimum movement direction values of AlienShips
	UPROPERTY(EditAnywhere)
	FVector m_minMovementDirection;

	//! Maximum movement direction values of AlienShips
	UPROPERTY(EditAnywhere)
	FVector m_maxMovementDirection;

	//! Minimum and maximum fire rate values in seconds
	UPROPERTY(EditAnywhere)
	FVector2D m_fireRateRange;
};
