// Fill out your copyright notice in the Description page of Project Settings.


#include "AsteroidSpawner.h"

#include "Kismet/GameplayStatics.h"

#include "Asteroids/BasicAsteroid.h"
#include "Player/SpaceshipPawn.h"
#include "Player\SpaceshipPlayerState.h"

AAsteroidSpawner::AAsteroidSpawner()
{
	m_player = nullptr;
}

void AAsteroidSpawner::OnSpawnActor(AActor* actor)
{
	ABasicAsteroid* asteroid = Cast<ABasicAsteroid>(actor);
	if (!asteroid)
		return;

	// Give random spin & movement direction
	asteroid->SetRandomDirections();

	// Set a random scale to asteroid
	float minScale = 1.0f;
	float maxScale = 6.0f;
	FVector scale = FVector(FMath::RandRange(minScale, maxScale), FMath::RandRange(minScale, maxScale), FMath::RandRange(minScale, maxScale));
	asteroid->SetActorScale3D(scale);

	// Give the asteroids an amount to give to the player once destroyed
	asteroid->SetRewardScore(5);

	// Listen to event on asteroid that gets triggered when the asteroid gets destroyed
	asteroid->OnAsteroidDestroyed.AddDynamic(this, &AAsteroidSpawner::OnAsteroidDestroyed);
}

void AAsteroidSpawner::OnAsteroidDestroyed(ABasicAsteroid* asteroid, TArray<ABasicAsteroid*> childAsteroids)
{
	if (m_player && asteroid != nullptr)
	{
		// Get the score and add to player's score of destroyed asteroid
		ASpaceshipPlayerState* state = Cast<ASpaceshipPlayerState>(m_player->GetPlayerState());
		if (state) {
			state->AddScore(asteroid->GetRewardScore());
			UE_LOG(LogTemp, Log, TEXT("'%s' destroyed. Player earning '%d' points"), *asteroid->GetName(), asteroid->GetRewardScore());
		}

		m_actorsCount -= 1;

		// If any child asteroids were spawned, add event listeners and repeat
		if (childAsteroids.Num() > 0) 
		{
			// Add child asteroids to count
			int childAsteroidCount = childAsteroids.Num();
			m_actorsCount += childAsteroidCount;
			for (int i = 0; i < childAsteroidCount; i++) 
			{
				if (childAsteroids.IsValidIndex(i)) {
					childAsteroids[i]->OnAsteroidDestroyed.AddDynamic(this, &AAsteroidSpawner::OnAsteroidDestroyed);
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to add score. No reference to Player!"));
	}
}

