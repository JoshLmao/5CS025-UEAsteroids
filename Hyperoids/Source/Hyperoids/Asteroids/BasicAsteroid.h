// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasicAsteroid.generated.h"

UCLASS()
class HYPEROIDS_API ABasicAsteroid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasicAsteroid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlap(AActor* overlappedActor, AActor* otherActor);

	UFUNCTION()
	void OnEndOverlap(AActor* overlappedActor, AActor* otherActor);

public:	
	static const FName ASTEROID_TAG;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Gives the asteroid a random movement direction and random rotation
	void SetRandomDirections();

	void SetMovementDirection(const FVector direction);
	void SetRandomLocation();

	void SetAsChildAsteroid();

	int GetRewardScore();
	void SetRewardScore(int amount);

private:
	// Current location of the Asteroid
	FVector m_location;
	// Is the asteroid at the edge of the world (screen)
	bool bEdgeOfWorld;
	// Amount of rotation to apply to the asteroid
	float m_rotationAmount;
	// Direction the asteroid will move
	FVector m_movementDirection;
	// Size of capsule collider for asteroid
	float m_colliderSize;
	bool m_bIsChildAsteroid;
	// Amount of points to reward the player for destroying asteroid
	int m_rewardScore;

	FVector GetRndVectorInBoundary(float maxX, float maxY);

	// Sets the overall size of the collider for this asteroid
	void SetColliderSize(float size);
	// Spawns the amount of child asteroids from the asteroid location
	void SpawnChildAsteroids(int amount);
};
