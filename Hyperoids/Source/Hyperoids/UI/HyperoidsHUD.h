// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HyperoidsHUD.generated.h"

/**
 * 
 */
UCLASS()
class HYPEROIDS_API AHyperoidsHUD : public AHUD
{
	GENERATED_BODY()

	class ASpaceshipPlayerState* PlayerState;

public:
	AHyperoidsHUD();

	UPROPERTY()
	UFont* m_hudFont;

	virtual void DrawHUD() override;
};
