// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AlienShipProjectile.generated.h"

UCLASS()
class HYPEROIDS_API AAlienShipProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAlienShipProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void OnOverlap(AActor* overlappedActor, AActor* otherActor);

	void SetMovementDirection(FVector moveDir);
private:
	FVector m_movementDirection;
	FVector2D m_playArea;
};
