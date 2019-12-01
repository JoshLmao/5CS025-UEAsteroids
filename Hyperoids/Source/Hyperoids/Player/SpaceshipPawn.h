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

	void MoveForwardInput(float value);
	void MoveRightInput(float value);
	void FireInput(float value);

	UFUNCTION()
	void OnOverlap(AActor* overlappedActor, AActor* otherActor);

	UFUNCTION()
	void OnEndOverlap(AActor* overlappedActor, AActor* otherActor);

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

	bool bEdgeOfWorld;
	bool m_bCanFire;
	float m_fireRate;
	float m_gunOffset;

	class USoundBase* m_fireSound;
	class UStaticMeshComponent* m_shipMeshComponent;

	FTimerHandle TimerHandle_ShotTimerExpired;

	void ShotTimerExpired();
	void BoundaryCheck(float deltaTime);
	void FireProjectile();
};
