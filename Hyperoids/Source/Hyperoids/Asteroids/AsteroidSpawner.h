// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"

#include "ActorSpawner.h"

#include "AsteroidSpawner.generated.h"


/**
 * 
 */
UCLASS()
class HYPEROIDS_API AAsteroidSpawner : public AActorSpawner
{
	GENERATED_BODY()
	
public:
	AAsteroidSpawner();

protected:
	virtual void OnSpawnActor(class AActor* actor) override;
	
private:
	UFUNCTION()
	void OnAsteroidDestroyed(class ABasicAsteroid* asteroid, TArray<class ABasicAsteroid*> childAsteroids);
};
