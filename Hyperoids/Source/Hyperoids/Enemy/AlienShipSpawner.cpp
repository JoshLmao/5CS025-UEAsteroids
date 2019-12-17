// Fill out your copyright notice in the Description page of Project Settings.


#include "AlienShipSpawner.h"

#include "Kismet/GameplayStatics.h"

#include "Enemy\AlienShip.h"
#include "Player\SpaceshipPawn.h"
#include "Player/SpaceshipPlayerState.h"

AAlienShipSpawner::AAlienShipSpawner()
{
}

void AAlienShipSpawner::OnSpawnActor(AActor* actor)
{
	AAlienShip* alien = Cast<AAlienShip>(actor);
	if (!alien)
		return;

	// Set rate of fire and how much score the player will recieve
	alien->SetFireRate(2.0f);
	alien->SetRewardScore(20);

	// Set the constant move direction of the AlienShip
	FVector moveDir;
	moveDir.Y = FMath::RandRange(100.0f, 200.0f);
	alien->SetMovementDirection(moveDir);

	alien->OnAlienDeath.AddDynamic(this, &AAlienShipSpawner::OnAlienShipDeath);
}

void AAlienShipSpawner::OnAlienShipDeath(AAlienShip* alienShip)
{
	if (m_player != nullptr && alienShip != nullptr)
	{
		ASpaceshipPlayerState* state = Cast<ASpaceshipPlayerState>(m_player->GetPlayerState());
		if (state)
			state->AddScore(alienShip->GetRewardScore());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to provide reward to player on AlienShipSpawner"));
	}
}
