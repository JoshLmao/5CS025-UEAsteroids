// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasicAsteroid.generated.h"

UCLASS()
class HYPEROIDS_API ABasicAsteroid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasicAsteroid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlap(AActor* overlappedActor, AActor* otherActor);

	UFUNCTION()
	void OnEndOverlap(AActor* overlappedActor, AActor* otherActor);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	class UStaticMesh* m_asteroidMesh;
};
