// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SpaceshipPawn.generated.h"

UCLASS()
class HYPEROIDS_API ASpaceshipPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASpaceshipPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;

	void MoveForwardInput(float value);
	void MoveRightInput(float value);
	void FireInput(float value);

	UFUNCTION()
	void OnOverlap(AActor* overlappedActor, AActor* otherActor);

	UFUNCTION()
	void OnEndOverlap(AActor* overlappedActor, AActor* otherActor);

	UFUNCTION(BlueprintCallable, Category="Player Score")
	int GetPlayerScore();

	UFUNCTION()
	void AddPlayerScore(int amount);

private:
	// Speed at which the player turns themselves
	UPROPERTY(EditAnywhere)
	float TurnSpeed = 500.0f;

	// Speed at which the player moves forward
	UPROPERTY(EditAnywhere)
	float ForwardSpeed = 500.0f;

	// Current movement direction of the player
	UPROPERTY(EditAnywhere)
	FVector MovementDirection = FVector::ZeroVector;

	// Speed of the projectile shot from the player
	UPROPERTY(EditAnywhere)
	float m_projectileSpeed;

	float m_currentForwardSpeed;
	float m_currentRotationSpeed;
	float m_currentFire;

	// Has the player reached the edge of our world/play area
	bool bEdgeOfWorld;
	// Can the player fire a projectile
	bool m_bCanFire;
	float m_fireRate;
	// Amount of units to spawn the projectile from the player's ship
	float m_gunOffset;
	// Amount of seconds to wait unbetween shooting projectiles
	float m_projectileWaitSeconds;
	// Current cooldown time between shooting projectiles
	float m_projectileTimer;
	// The current score of the player 
	int m_score;

	class USoundBase* m_fireSound;
	class USoundBase* m_playerDeathSound;

	class UStaticMeshComponent* m_shipMeshComponent;

	FTimerHandle TimerHandle_ShotTimerExpired;

	void ShotTimerExpired();
	void BoundaryCheck(float deltaTime);
	void FireProjectile();
};
