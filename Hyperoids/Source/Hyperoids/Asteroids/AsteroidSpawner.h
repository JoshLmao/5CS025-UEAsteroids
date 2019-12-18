// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"

#include "ActorSpawner.h"

#include "AsteroidSpawner.generated.h"


/**
 * Handles spawning of asteroids within an area, while also ammending the player's score
 * once an asteroid has been destroyed
 */
UCLASS()
class HYPEROIDS_API AAsteroidSpawner : public AActorSpawner
{
	GENERATED_BODY()
	
public:
	AAsteroidSpawner();

protected:
	//! Overrides the OnSpawnActor method to implement asteroid specific code
	virtual void OnSpawnActor(class AActor* actor) override;
	
private:
	//! Handles functionality for when an asteroid has been destroyed by the player
	UFUNCTION()
	void OnAsteroidDestroyed(class ABasicAsteroid* asteroid, TArray<class ABasicAsteroid*> childAsteroids);
};
