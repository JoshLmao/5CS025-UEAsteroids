// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasicAsteroid.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBasicAsteroidDestroyedSignature, ABasicAsteroid*, Asteroid, TArray<ABasicAsteroid*>, ChildAsteroids);

/**
 * Simple asteroid that can spin and move around in a constant direction inside a play area
 */
UCLASS()
class HYPEROIDS_API ABasicAsteroid : public AActor
{
	GENERATED_BODY()
	
public:	
	//! Sets default values for this actor's properties
	ABasicAsteroid();

protected:
	//! Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlap(AActor* overlappedActor, AActor* otherActor);

public:
	//! Called every frame
	virtual void Tick(float DeltaTime) override;
	//! Gives the asteroid a random movement direction and random rotation
	void SetRandomDirections();

	//! Gets the current direction of movement of the asteroid
	FVector GetMovementDirection();
	//! Sets the movement direction of this asteroid
	void SetMovementDirection(const FVector direction);

	/** Mark this asteroid as a child of another asteroid that has already been destroyed
	 If marked as a child, this asteroid will not spawn any more asteroids once destroyed */
	void SetAsChildAsteroid();

	//! Gets the amount of score to give to the player once destroyed
	int GetRewardScore();
	//! Sets the amount of score to give to the player on destroyed
	void SetRewardScore(int amount);

	//! Returns a random vector inside the given play area */
	static FVector2D GetRndVectorInBoundary(FVector2D playArea);
	//! Returns a random vector inside the given area that is outside the safe area (Hollow ractangle inside playArea)
	static FVector2D GetRndVectorInBoundary(FVector2D playArea, FVector2D safeArea);

	//! Event for listenting to when this asteroid gets destroyed
	UPROPERTY(BlueprintAssignable)
	FBasicAsteroidDestroyedSignature OnAsteroidDestroyed;

private:
	//! Current location of the Asteroid
	FVector m_location;
	//! Total current play area of the game
	FVector2D m_playArea;
	//! Is the asteroid at the edge of the world (screen)
	bool bEdgeOfWorld;
	//! Amount of rotation to apply to the asteroid
	float m_rotationAmount;
	//! Direction the asteroid will move
	FVector m_movementDirection;
	//! Size of capsule collider for asteroid
	float m_colliderSize;
	/** Flag to determine if this asteroid has been created with the purpose of being a child asteroid
		If so, this asteroid will not spawn any more child asteroids */
	bool m_bIsChildAsteroid;
	//! Amount of points to reward the player for destroying asteroid
	int m_rewardScore;

	//! Explosion sound to play once asteroid is destroyed
	class USoundBase* m_explodeSound;
	//! Particle system to use once the asteroid is destroyed
	class UParticleSystem* m_explodePS;

	//! Sets the overall size of the collider for this asteroid
	void SetColliderSize(float size);
	//! Spawns the amount of child asteroids from the asteroid location
	TArray<ABasicAsteroid*> SpawnChildAsteroids(int amount);
};
