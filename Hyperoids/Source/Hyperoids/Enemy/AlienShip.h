// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AlienShip.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAlienShipOnDeathSignature, AAlienShip*, Alien);

UCLASS()
class HYPEROIDS_API AAlienShip : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAlienShip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlap(AActor* overlappedActor, AActor* otherActor);

	// Event to represent when the AlienShip has been killed by the player
	UPROPERTY(BlueprintAssignable)
	FAlienShipOnDeathSignature OnAlienDeath;

	// Get the current movement direction of the AlienShip
	FVector GetMovementDirection();
	// Set the movement direction of the AlienShip
	void SetMovementDirection(FVector direction);

	// Gets the current fire rate in seconds of the AlienShip
	float GetFireRate();
	// Sets the new fire rate in seconds of the AlienShip
	void SetFireRate(float fireRateSeconds);

	// Gets the total reward score to give the player
	int GetRewardScore();
	// Set a new reward score to give to the player on death
	void SetRewardScore(int score);

private:
	// Sound for the AlienShip when firing
	class USoundBase* m_fireSound;
	// Sound for the AlienShip on death
	class USoundBase* m_deathSound;

	// Mesh Component visual for the AlienShip
	class UStaticMeshComponent* m_alienComponent;
	// Particle system to use once AlienShip is destroyed
	class UParticleSystem* m_explosionPS;

	// Current main player in the game
	class ASpaceshipPawn* m_player;

	// Current location of the alien ship
	FVector m_location;
	// Is the AlienShip at the edge of the world
	bool m_bEdgeOfWorld;
	// Is the AlienShip currently alive and shooting at the player
	bool m_bIsAlive;
	// Can the AlienShip fire at the player
	bool m_bCanFire;

	// The fire rate in seconds of the AlienShip
	float m_fireRate;
	// The speed of the projectile from the AlienShip
	float m_projectileSpeed;
	// Current cooldown timer of the AlienShip firing
	float m_projectileTimer;

	// Current movement direction of the AlienShip
	UPROPERTY(EditAnywhere)
	FVector m_movementDirection;

	// Total play area coordinates
	FVector2D m_playArea;

	// Amount of score to reward the player with on death
	UPROPERTY(EditAnywhere)
	int m_rewardScore;

	// Process movement of the AlienShip per tick
	void Move(float deltaTime);
	// Process Firing at player per tick
	void FireAtPlayer();
};
