// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

/**
 * Base class for projectiles. Able to move constantly in a direction 
 */
UCLASS()
class HYPEROIDS_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	//! Sets default values for this actor's properties
	AProjectileBase();

protected:
	//! Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	//! Called every frame
	virtual void Tick(float DeltaTime) override;

	//! Virtual function for determining when the projectile collides with another actor
	UFUNCTION()
	virtual void OnOverlap(AActor* overlappedActor, AActor* otherActor);

	//! Sets the current moving direction of the projectile
	void SetMovementDirection(FVector moveDir);
	//! Gets the current moving direction of the projectile
	FVector GetMovementDirection();

private:
	//! Current moving direction of the projectile
	FVector m_movementDirection;
	//! Total play area to stay inside
	FVector2D m_playArea;
};
