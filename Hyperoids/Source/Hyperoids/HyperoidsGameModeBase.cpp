// Fill out your copyright notice in the Description page of Project Settings.


#include "HyperoidsGameModeBase.h"

#include "Kismet/GameplayStatics.h"

#include "Player/SpaceshipPawn.h"
#include "UI/HyperoidsHUD.h"
#include "Player/SpaceshipPlayerState.h"
#include "AsteroidSpawner.h"

AHyperoidsGameModeBase::AHyperoidsGameModeBase()
{
	// Set default classes for game objects
	DefaultPawnClass = ASpaceshipPawn::StaticClass();
	PlayerStateClass = ASpaceshipPlayerState::StaticClass();
	HUDClass = AHyperoidsHUD::StaticClass();
}

void AHyperoidsGameModeBase::StartPlay()
{
	Super::StartPlay();

	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAsteroidSpawner::StaticClass(), foundActors);

	if (foundActors.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to find an AsteroidSpawner!"));
	}
	else 
	{
		for (AActor* actor : foundActors)
		{
			AAsteroidSpawner* spawner = Cast<AAsteroidSpawner>(actor);
			if (spawner != nullptr)
			{
				m_playArea = spawner->GetPlayArea();
				return;
			}
		}
	}
}

FVector2D AHyperoidsGameModeBase::GetPlayArea()
{
	return m_playArea;
}
