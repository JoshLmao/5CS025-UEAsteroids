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

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMesh(TEXT("StaticMesh'/Game/TwinStick/Meshes/TwinStickProjectile.TwinStickProjectile'"));
	if (ProjectileMesh.Succeeded())
	{
		meshComponent->SetStaticMesh(ProjectileMesh.Object);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No mesh found for AlienShipProjectile!"));
	}

	OnActorBeginOverlap.AddDynamic(this, &AAlienShipProjectile::OnOverlap);
}

// Called when the game starts or when spawned
void AAlienShipProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	AHyperoidsGameModeBase* gm = (AHyperoidsGameModeBase*)GetWorld()->GetAuthGameMode();
	m_playArea = gm->GetPlayArea();
}

// Called every frame
void AAlienShipProjectile::Tick(float DeltaTime)
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
	if (bAtEdge)
		Destroy();
}

void AAlienShipProjectile::OnOverlap(AActor* overlappedActor, AActor* otherActor)
{
	if (otherActor->IsA(ASpaceshipPawn::StaticClass()))
	{
		UE_LOG(LogTemp, Log, TEXT("Enemy Ship projectile collided with Player Pawn"));
		this->Destroy();
	}
}

void AAlienShipProjectile::SetMovementDirection(FVector moveDir)
{
	m_movementDirection = moveDir;
}