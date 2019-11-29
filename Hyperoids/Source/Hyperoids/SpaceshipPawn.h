// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SpaceshipPawn.generated.h"

UCLASS()
class HYPEROIDS_API ASpaceshipPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASpaceshipPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForwardInput(float value);
	void MoveRightInput(float value);
	void FireInput(float value);

private:
	float m_currentForwardSpeed;
	float m_currentRotationSpeed;
	float m_currentFire;

	bool m_bCanFire;
	float m_fireRate;
	float m_gunOffset;

	UPROPERTY(EditAnywhere)
	class UStaticMesh* m_shipModel;

	UPROPERTY(EditAnywhere)
	class USoundBase* m_fireSound;

	class UStaticMeshComponent* m_shipMeshComponent;

	FTimerHandle TimerHandle_ShotTimerExpired;

	void ShotTimerExpired();
};
