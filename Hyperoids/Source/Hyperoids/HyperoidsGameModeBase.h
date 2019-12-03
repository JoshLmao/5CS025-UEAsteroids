// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Asteroids/BasicAsteroid.h"

#include "HyperoidsGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathSignature, class AActor*, actor);

/**
 * 
 */
UCLASS()
class HYPEROIDS_API AHyperoidsGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AHyperoidsGameModeBase();

	/* Gets the size of the current play area. FVector2D returns top-most & right-most vector*/
	FVector2D GetPlayArea();
	/* Returns the area coordinates at which the player will spawn inside */
	FVector2D GetSpawnArea();

	static const int MAX_ASTEROIDS;

	void OnAsteroidDestroyed(class ABasicAsteroid* asteroid);

	void PlayerDeath(AActor* player);

	UPROPERTY(BlueprintAssignable, Category="Player Event")
	FOnDeathSignature OnPlayerDeath;

private:
	TArray<AActor*> m_asteroids;
	
	FVector2D m_playArea;
	FVector2D m_spawnArea;

	class ASpaceshipPawn* m_player;

	FTimerHandle SpawnHandle_CreateAsteroids;

	virtual void StartPlay() override;

	UFUNCTION()
	void SpawnAsteroids();

	FVector GetRndVectorInBoundary(float maxX, float maxY);
};
