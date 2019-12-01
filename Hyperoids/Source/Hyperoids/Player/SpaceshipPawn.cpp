// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceshipPawn.h"

#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/CollisionProfile.h"
#include "GameFramework/PlayerController.h"

#include "Asteroids/BasicAsteroid.h"
#include "HyperoidsGameModeBase.h"
#include "Player\ShipProjectile.h"

// Sets default values
ASpaceshipPawn::ASpaceshipPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_shipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ship Mesh"));
	m_shipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	RootComponent = m_shipMeshComponent;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("StaticMesh'/Game/ExampleContent/Input_Examples/Meshes/SM_UFO_Main.SM_UFO_Main'"));
	if (ShipMesh.Succeeded())
	{
		m_shipMeshComponent->SetStaticMesh(ShipMesh.Object);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Mesh has been set for the player!"));
	}

	m_currentForwardSpeed = 0.0f;
	m_currentRotationSpeed = 0.0f;
	m_currentFire = 0.0f;
	m_projectileSpeed = 750.0f;

	m_bCanFire = true;
	m_fireRate = 1.0f;
	m_gunOffset = 70.0f;

	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("SoundWave'/Game/TwinStick/Audio/TwinStickFire.TwinStickFire'"));
	if (FireAudio.Succeeded())
		m_fireSound = FireAudio.Object;

	OnActorBeginOverlap.AddDynamic(this, &ASpaceshipPawn::OnOverlap);
	OnActorEndOverlap.AddDynamic(this, &ASpaceshipPawn::OnEndOverlap);
}

// Called when the game starts or when spawned
void ASpaceshipPawn::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASpaceshipPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FVector LocalMove = FVector(m_currentForwardSpeed * DeltaTime, 0.f, 0.f);
	AddActorLocalOffset(LocalMove, true);

	FRotator DeltaRotation(0, 0, 0);
	DeltaRotation.Yaw = m_currentRotationSpeed * DeltaTime;
	AddActorLocalRotation(DeltaRotation);

	BoundaryCheck(DeltaTime);

	if (m_currentFire > 0.0f && m_bCanFire)
	{
		FireProjectile();
	}
}

// Called to bind functionality to input
void ASpaceshipPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("MoveForward", this, &ASpaceshipPawn::MoveForwardInput);
	InputComponent->BindAxis("MoveRight", this, &ASpaceshipPawn::MoveRightInput);
	InputComponent->BindAxis("FireForward", this, &ASpaceshipPawn::FireInput);
}

void ASpaceshipPawn::MoveForwardInput(float value)
{
	if (!FMath::IsNearlyEqual(value, 0.0f))
		m_currentForwardSpeed = value * ForwardSpeed;
	else
		m_currentForwardSpeed = 0.0f;
}

void ASpaceshipPawn::MoveRightInput(float value)
{
	if (!FMath::IsNearlyEqual(value, 0.0f))
		m_currentRotationSpeed = value * TurnSpeed;
	else
		m_currentRotationSpeed = 0.0f;
}

void ASpaceshipPawn::FireInput(float value)
{
	if (!FMath::IsNearlyEqual(value, 0.0f))
		m_currentFire = value;
	else
		m_currentFire = 0.0f;
}

void ASpaceshipPawn::ShotTimerExpired()
{
	m_bCanFire = true;
}

void ASpaceshipPawn::BoundaryCheck(float deltaTime)
{
	FVector playerLoc = GetActorLocation();

	// Works out if the world location is inside screen space location
	UWorld* world = GetWorld();
	AHyperoidsGameModeBase* gm = (AHyperoidsGameModeBase*)world->GetAuthGameMode();
	FVector2D playArea = gm->GetPlayArea();

	// Validate X is within boundary of play area
	if (playerLoc.X < -playArea.X)
	{
		playerLoc.X = playArea.X;
		bEdgeOfWorld = true;
	}
	if (playerLoc.X > playArea.X)
	{
		playerLoc.X = -playArea.X;
		bEdgeOfWorld = true;
	}

	// Validate Y is within boundary of play area
	if (playerLoc.Y < -playArea.Y)
	{
		playerLoc.Y = playArea.Y;
		bEdgeOfWorld = true;
	}
	if (playerLoc.Y > playArea.Y)
	{
		playerLoc.Y = -playArea.Y;
		bEdgeOfWorld = true;
	}

	SetActorLocation(playerLoc + MovementDirection * deltaTime, !bEdgeOfWorld);
}

void ASpaceshipPawn::OnOverlap(AActor* overlappedActor, AActor* otherActor)
{
	FName tag = "";
	if (otherActor->Tags.Num() > 0)
		tag = otherActor->Tags[0];

	if (tag == ABasicAsteroid::ASTEROID_TAG)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player has died!"));
	}
}

void ASpaceshipPawn::OnEndOverlap(AActor* overlappedActor, AActor* otherActor)
{

}

void ASpaceshipPawn::FireProjectile()
{
	m_bCanFire = false;
	
	UWorld* world = GetWorld();
	const FVector loc = GetActorLocation();
	const FRotator rot = GetActorRotation();
	// Spawn projectile infront of ship with it's position & rotation
	AShipProjectile* projectile = (AShipProjectile*)world->SpawnActor<AShipProjectile>(AShipProjectile::StaticClass(), loc, rot);
	// Add offset to projectile so to now spawn inside player
	projectile->AddActorLocalOffset(FVector(m_gunOffset, 0.0f, 0.0f));

	// Set movement vector to be forward position of player
	FVector forward = GetActorForwardVector();
	projectile->SetMovementDirection(forward * m_projectileSpeed);
	UE_LOG(LogTemp, Log, TEXT("Forward direction: %s"), *projectile->GetActorLocation().ToString());
}