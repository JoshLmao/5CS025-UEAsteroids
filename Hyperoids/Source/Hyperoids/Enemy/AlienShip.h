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

	UPROPERTY(BlueprintAssignable)
	FAlienShipOnDeathSignature OnAlienDeath;

	void SetMovementDirection(FVector direction);
	FVector GetMovementDirection();

	float GetFireRate();
	void SetFireRate(float fireRateSeconds);

	int GetRewardScore();
	void SetRewardScore(int score);

private:
	class USoundBase* m_fireSound;
	class USoundBase* m_deathSound;

	class UStaticMeshComponent* m_alienComponent;
	class UParticleSystem* m_explosionPS;

	FVector m_location;
	bool m_bEdgeOfWorld;
	bool m_bIsAlive;
	float m_fireRate;
	FVector m_movementDirection;
	FVector2D m_playArea;
	int m_rewardScore;

	void Move(float deltaTime);
	void FireAtPlayer();
};
