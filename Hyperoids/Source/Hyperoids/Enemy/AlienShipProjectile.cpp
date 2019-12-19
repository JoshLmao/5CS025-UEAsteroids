// Fill out your copyright notice in the Description page of Project Settings.


#include "AlienShipProjectile.h"

#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"

#include "Player\SpaceshipPawn.h"
#include "HyperoidsGameModeBase.h"

// Sets default values
AAlienShipProjectile::AAlienShipProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UStaticMeshComponent* meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh Component"));
	meshComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	meshComponent->SetGenerateOverlapEvents(true);
	RootComponent = meshComponent;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMesh(TEXT("StaticMesh'/Game/Models/Projectile/Projectile.Projectile'"));
	if (ProjectileMesh.Succeeded())
	{
		meshComponent->SetStaticMesh(ProjectileMesh.Object);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No mesh found for AlienShipProjectile!"));
	}
}

void AAlienShipProjectile::OnOverlap(AActor* overlappedActor, AActor* otherActor)
{
	if (otherActor->IsA(ASpaceshipPawn::StaticClass()))
	{
		UE_LOG(LogTemp, Log, TEXT("EnemyShip projectile collided with '%s'"), *otherActor->GetName());
		this->Destroy();
	}
}
