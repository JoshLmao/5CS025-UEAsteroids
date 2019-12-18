// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SpaceshipPawn.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpaceshipPawnOnDeathSignature, class ASpaceshipPawn*, actor);

/**
 * Main Player's spaceship that can fire and destroy asteroids or AlienShips
 */
UCLASS()
class HYPEROIDS_API ASpaceshipPawn : public APawn
{
	GENERATED_BODY()

public:
	//! Sets default values for this pawn's properties
	ASpaceshipPawn();

protected:
	//! Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	//! Called every frame
	virtual void Tick(float DeltaTime) override;

	//! Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//! Called once any forward axis input is recieved from the engine
	void MoveForwardInput(float value);
	//! Called once any horizontal axis input is recieved from the engine
	void MoveRightInput(float value);
	//! Called once any fire input is recieved from the engine
	void FireInput(float value);

	//! Overlap detection to determine if the player gets killed
	UFUNCTION()
	void OnOverlap(AActor* overlappedActor, AActor* otherActor);

	//! Event for broadcasting once the player has died
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FSpaceshipPawnOnDeathSignature OnPlayerDeath;

	//! Gets if the player is alive or dead
	bool GetIsAlive();

private:
	//! Speed at which the player turns themselves
	UPROPERTY(EditAnywhere)
	float TurnSpeed = 500.0f;

	//! Speed at which the player moves forward
	UPROPERTY(EditAnywhere)
	float ForwardSpeed = 500.0f;

	//! Current movement direction of the player
	UPROPERTY(EditAnywhere)
	FVector MovementDirection = FVector::ZeroVector;

	//! Speed of the projectile shot from the player
	UPROPERTY(EditAnywhere)
	float m_projectileSpeed;

	//! Current vertical axis input into the engine
	float m_currentForwardSpeed;
	//! Current horizontal axis input into the engine
	float m_currentRotationSpeed;
	//! Current fire input into the engine
	float m_currentFire;

	//! Has the player reached the edge of our world/play area
	bool bEdgeOfWorld;
	//! Can the player fire a projectile
	bool m_bCanFire;
	//! If the player is alive or dead
	bool m_bIsAlive;

	//! Amount of units to spawn the projectile from the player's ship
	float m_gunOffset;
	//! Amount of seconds to wait unbetween shooting projectiles
	float m_fireRate;
	//! Current cooldown time between shooting projectiles
	float m_projectileTimer;

	//! Sound to play when the player fires a projectile
	class USoundBase* m_fireSound;
	//! Death sound to play once the player has died
	class USoundBase* m_playerDeathSound;

	//! Visual mesh of the player's ship
	class UStaticMeshComponent* m_shipMeshComponent;
	//! Particle system to use to represent an explosion when the player dies
	class UParticleSystem* m_explosionPS;

	//! Current play area of the game
	FVector2D m_playArea;

	//! Validate if the current player is inside the safe play area of the game
	void BoundaryCheck(float deltaTime);
	//! Fires a projectile in the direction the player is facing
	void FireProjectile();
};
