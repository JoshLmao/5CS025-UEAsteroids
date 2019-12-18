// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ProjectileBase.h"

#include "ShipProjectile.generated.h"

UCLASS()
class HYPEROIDS_API AShipProjectile : public AProjectileBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShipProjectile();

	virtual void OnOverlap(AActor* overlappedActor, AActor* otherActor) override;
};
