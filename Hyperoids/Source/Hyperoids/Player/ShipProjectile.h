// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShipProjectile.generated.h"

UCLASS()
class HYPEROIDS_API AShipProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShipProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlap(AActor* overlappedActor, AActor* otherActor);

	UFUNCTION()
	void OnEndOverlap(AActor* overlappedActor, AActor* otherActor);

public:	
	static const FName PROJECTILE_TAG;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetMovementDirection(FVector moveDir);

private:
	FVector m_movementDirection;
	FVector2D m_playArea;
};
