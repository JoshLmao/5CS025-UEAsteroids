// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipProjectile.h"

#include "Components/CapsuleComponent.h"
#include "ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"

#include "Asteroids\BasicAsteroid.h"
#include "HyperoidsGameModeBase.h"

const FName AShipProjectile::PROJECTILE_TAG = TEXT("projectile");

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

	this->Tags.Add(PROJECTILE_TAG);

	// Add Overlap listeners to detect when hitting asteroid
	OnActorBeginOverlap.AddDynamic(this, &AShipProjectile::OnOverlap);
	OnActorEndOverlap.AddDynamic(this, &AShipProjectile::OnEndOverlap);
}

// Called when the game starts or when spawned
void AShipProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShipProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector location = GetActorLocation();
	AHyperoidsGameModeBase* gm = (AHyperoidsGameModeBase*)GetWorld()->GetAuthGameMode();
	FVector2D playArea = gm->GetPlayArea();
	
	bool bAtEdge = false;
	if (location.X < -playArea.X)
		bAtEdge = true;
	if (location.X > playArea.X)
		bAtEdge = true;
	if (location.Y < -playArea.Y)
		bAtEdge = true;
	if (location.Y > playArea.Y)
		bAtEdge = true;

	SetActorLocation(location + m_movementDirection * DeltaTime, false);
	if (bAtEdge)
		Destroy();
}

void AShipProjectile::OnOverlap(AActor* overlappedActor, AActor* otherActor)
{
	FName tag;
	if (otherActor->Tags.Num() > 0)
		tag = otherActor->Tags[0];

	if (tag == ABasicAsteroid::ASTEROID_TAG)
	{
		UE_LOG(LogTemp, Log, TEXT("Projectile collided with Asteroid - Blow asteroid up"));
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