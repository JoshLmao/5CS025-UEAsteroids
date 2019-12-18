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

	// Add Overlap listeners to detect when hitting asteroid
	OnActorBeginOverlap.AddDynamic(this, &AShipProjectile::OnOverlap);
	OnActorEndOverlap.AddDynamic(this, &AShipProjectile::OnEndOverlap);
}

// Called when the game starts or when spawned
void AShipProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	AHyperoidsGameModeBase* gm = (AHyperoidsGameModeBase*)GetWorld()->GetAuthGameMode();
	m_playArea = gm->GetPlayArea();
}

// Called every frame
void AShipProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector location = GetActorLocation();
	
	bool bAtEdge = false;
	if (location.X < -m_playArea.X)
		bAtEdge = true;
	if (location.X > m_playArea.X)
		bAtEdge = true;
	if (location.Y < -m_playArea.Y)
		bAtEdge = true;
	if (location.Y > m_playArea.Y)
		bAtEdge = true;

	SetActorLocation(location + m_movementDirection * DeltaTime, false);
	if (bAtEdge) {
		//Destroy();
	}
}

void AShipProjectile::OnOverlap(AActor* overlappedActor, AActor* otherActor)
{
	if (otherActor->IsA(ABasicAsteroid::StaticClass()) || otherActor->IsA(AAlienShip::StaticClass()))
	{
		UE_LOG(LogTemp, Log, TEXT("Projectile collided with a game object"));
		this->Destroy();
	}
}

void AShipProjectile::OnEndOverlap(AActor* overlappedActor, AActor* otherActor)
{

}

void AShipProjectile::SetMovementDirection(FVector moveDir)
{
	m_movementDirection = moveDir;
}