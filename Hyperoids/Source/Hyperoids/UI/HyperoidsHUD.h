// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HyperoidsHUD.generated.h"

/**
 * UI required for the Hyperoids game. Builds the player's score and updates all UI
 */
UCLASS()
class HYPEROIDS_API AHyperoidsHUD : public AHUD
{
	GENERATED_BODY()

	//! Player's current state
	class ASpaceshipPlayerState* PlayerState;

public:
	AHyperoidsHUD();

	//! The font to use for the HUD
	UPROPERTY()
	UFont* m_hudFont;

	//! Override to draw custom UI
	virtual void DrawHUD() override;
};
