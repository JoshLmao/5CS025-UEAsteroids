// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipProjectile.h"

#include "Components/CapsuleComponent.h"
#include "ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"

#include "Asteroids\BasicAsteroid.h"
#include "Enemy/AlienShip.h"
#include "HyperoidsGameModeBase.h"

// Sets default values
AShipProjectile::AShipProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set projectile mesh
	UStaticMeshComponent* meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	meshComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	meshComponent->SetGenerateOverlapEvents(true);
	RootComponent = meshComponent;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMesh(TEXT("StaticMesh'/Game/TwinStick/Meshes/TwinStickProjectile.TwinStickProjectile'"));
	if (ProjectileMesh.Succeeded())
	{
		meshComponent->SetStaticMesh(ProjectileMesh.Object);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No mesh found for Projectile!"));
	}
}

void AShipProjectile::OnOverlap(AActor* overlappedActor, AActor* otherActor)
{
	if (otherActor->IsA(ABasicAsteroid::StaticClass()) || otherActor->IsA(AAlienShip::StaticClass()))
	{
		UE_LOG(LogTemp, Log, TEXT("PlayerProjectile collided with '%s'"), *otherActor->GetName());
		this->Destroy();
	}
}
