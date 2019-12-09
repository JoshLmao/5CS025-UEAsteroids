// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceshipPlayerState.h"

ASpaceshipPlayerState::ASpaceshipPlayerState()
{
	Score = 0;
}

int ASpaceshipPlayerState::GetScore()
{
	return Score;
}

void ASpaceshipPlayerState::SetScore(int score)
{
	Score = score;
}

void ASpaceshipPlayerState::AddScore(int amount)
{
	Score += amount;
}

void ASpaceshipPlayerState::RemoveScore(int amount)
{
	if (Score - amount < 0)
		Score = 0;
	else
		Score -= amount;
}
