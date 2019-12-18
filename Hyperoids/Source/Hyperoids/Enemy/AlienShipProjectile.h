// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.h"
#include "AlienShipProjectile.generated.h"

UCLASS()
class HYPEROIDS_API AAlienShipProjectile : public AProjectileBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAlienShipProjectile();

	virtual void OnOverlap(AActor* overlappedActor, AActor* otherActor) override;
};
