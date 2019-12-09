// Fill out your copyright notice in the Description page of Project Settings.


#include "HyperoidsHUD.h"

#include "UObject/ConstructorHelpers.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HyperoidsHUD.h"
#include "Player/SpaceshipPlayerState.h"

#define LOCTEXT_NAMESPACE "HyperoidsHUD"

AHyperoidsHUD::AHyperoidsHUD()
{
	static ConstructorHelpers::FObjectFinder<UFont>Font(TEXT("/Engine/EngineFonts/RobotoDistanceField"));
	m_hudFont = Font.Object;}

void AHyperoidsHUD::DrawHUD()
{
	Super::DrawHUD();

	if (PlayerState)
	{
		FVector2D ScreenDimensions = FVector2D(Canvas->SizeX, Canvas->SizeY);
		FText ScoreString = FText::Format(LOCTEXT("TestFormat", "Score: {0}"), FText::AsNumber(PlayerState->GetScore()));

		FVector2D TextCentrePos = FVector2D(20, 20);
		FCanvasTextItem TextItem(TextCentrePos, ScoreString, m_hudFont, FLinearColor::White);
		Canvas->DrawItem(TextItem);
	}
	else
	{
		APawn* player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (player)
			PlayerState = Cast<ASpaceshipPlayerState>(player->GetPlayerState());
	}
}

#undef LOCTEXT_NAMESPACE