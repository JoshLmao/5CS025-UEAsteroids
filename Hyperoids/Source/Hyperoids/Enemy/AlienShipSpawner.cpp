// Fill out your copyright notice in the Description page of Project Settings.


#include "AlienShipSpawner.h"

#include "Kismet/GameplayStatics.h"

#include "Enemy\AlienShip.h"
#include "Player\SpaceshipPawn.h"
#include "Player/SpaceshipPlayerState.h"

AAlienShipSpawner::AAlienShipSpawner()
{
	// Default values of minimum and maximum movement directions
	m_minMovementDirection = FVector(0.0f, 100.0f, 0.0f);
	m_maxMovementDirection = FVector(0.0f, 250.0f, 0.0f);

	// Default fire rate range values, from 7 to 10 seconds
	m_fireRateRange = FVector2D(7.0f, 10.0f);
}

void AAlienShipSpawner::OnSpawnActor(AActor* actor)
{
	// Cast spawned actor to alien ship
	AAlienShip* alien = Cast<AAlienShip>(actor);
	if (!alien)
		return;

	// Set rate of fire and how much score the player will recieve
	float fireRate = FMath::RandRange(m_fireRateRange.X, m_fireRateRange.Y);
	alien->SetFireRate(fireRate);

	alien->SetRewardScore(20);

	// Set the constant move direction of the AlienShip from Min and Max UProperties
	float x = FMath::RandRange(m_minMovementDirection.X, m_maxMovementDirection.X);
	float y = FMath::RandRange(m_minMovementDirection.Y, m_maxMovementDirection.Y);
	float z = FMath::RandRange(m_minMovementDirection.Z, m_maxMovementDirection.Z);
	FVector moveDir(x, y, z);
	alien->SetMovementDirection(moveDir);

	// Add listener to death event to handle adding score
	alien->OnAlienDeath.AddDynamic(this, &AAlienShipSpawner::OnAlienShipDeath);
}

void AAlienShipSpawner::OnAlienShipDeath(AAlienShip* alienShip)
{
	if (m_player != nullptr && alienShip != nullptr)
	{
		// Get player state and add to players score
		ASpaceshipPlayerState* state = Cast<ASpaceshipPlayerState>(m_player->GetPlayerState());
		if (state) {
			state->AddScore(alienShip->GetRewardScore());
			UE_LOG(LogTemp, Log, TEXT("'%s' destroyed. Player earning '%d' points"), *alienShip->GetName(), alienShip->GetRewardScore());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to provide reward to player on '%s'"), *GetName());
	}
}
