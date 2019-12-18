// Fill out your copyright notice in the Description page of Project Settings.


#include "AlienShip.h"

#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "Player\ShipProjectile.h"
#include "Enemy/AlienShipProjectile.h"
#include "HyperoidsGameModeBase.h"
#include "Player/SpaceshipPawn.h"

// Sets default values
AAlienShip::AAlienShip()
{
	m_bIsAlive = true;
	m_bCanFire = false;
	m_projectileSpeed = 1000.0f;
	m_projectileTimer = 0.0f;
	m_fireRate = 8.0f;

	m_movementDirection = FVector(0.0f, 0.0f, 0.0f);
	m_rewardScore = 25;
	m_playArea = FVector2D();

	PrimaryActorTick.bCanEverTick = true;

	m_alienComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Alien Mesh"));
	m_alienComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	m_alienComponent->SetGenerateOverlapEvents(true);
	RootComponent = m_alienComponent;

	// Set Alien Mesh to be correctly facing the top down camera
	FRotator rotation;
	rotation.Roll = -90.0f; // X
	rotation.Pitch = 0.0f;
	rotation.Yaw = 90.0f; // Z
	m_alienComponent->SetRelativeRotation(rotation);
	// Scale mesh up
	m_alienComponent->SetWorldScale3D(FVector(3.0f, 3.0f, 3.0f));

	ConstructorHelpers::FObjectFinder<UStaticMesh> AlienMesh(TEXT("StaticMesh'/Game/ExampleContent/Input_Examples/Meshes/SM_Pixel_Enemy_1.SM_Pixel_Enemy_1'"));
	if (AlienMesh.Succeeded())
		m_alienComponent->SetStaticMesh(AlienMesh.Object);

	ConstructorHelpers::FObjectFinder<USoundBase> AlienDeathSound(TEXT("SoundWave'/Game/Sounds/alien_death.alien_death'"));
	if (AlienDeathSound.Succeeded())
		m_deathSound = AlienDeathSound.Object;

	ConstructorHelpers::FObjectFinder<USoundBase> AlienFireSound(TEXT("SoundWave'/Game/Sounds/alien_fire.alien_fire'"));
	if (AlienFireSound.Succeeded())
		m_fireSound = AlienFireSound.Object;

	ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionPS(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	if (ExplosionPS.Succeeded())
		m_explosionPS = ExplosionPS.Object;

	OnActorBeginOverlap.AddDynamic(this, &AAlienShip::OnOverlap);
}

// Called when the game starts or when spawned
void AAlienShip::BeginPlay()
{
	Super::BeginPlay();
	
	AHyperoidsGameModeBase* gm = (AHyperoidsGameModeBase*)GetWorld()->GetAuthGameMode();
	m_playArea = gm->GetPlayArea();

	m_player = (ASpaceshipPawn*)UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

// Called every frame
void AAlienShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move(DeltaTime);

	if (m_bCanFire) 
	{
		FireAtPlayer();
	}
	else
	{
		m_projectileTimer += DeltaTime;
		if (m_projectileTimer >= m_fireRate) {
			m_bCanFire = true;
			m_projectileTimer = 0.0f;
		}
	}
}

void AAlienShip::OnOverlap(AActor* overlappedActor, AActor* otherActor)
{
	if (otherActor->IsA(AShipProjectile::StaticClass()))
	{
		UE_LOG(LogTemp, Log, TEXT("Alien has been killed by projectile!"));
		m_bIsAlive = false;

		// Spawn explosion particle
		if (m_explosionPS) {
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_explosionPS, GetActorLocation());
		}

		// Play alien death sound
		if (m_deathSound) {
			UGameplayStatics::PlaySoundAtLocation(this, m_deathSound, GetActorLocation());
		}

		// Trigger Alien Death event
		if (OnAlienDeath.IsBound())
			OnAlienDeath.Broadcast(this);

		// Finally destroy actor once completed
		Destroy();
	}
}

void AAlienShip::SetMovementDirection(FVector direction)
{
	m_movementDirection = direction;
}

FVector AAlienShip::GetMovementDirection()
{
	return m_movementDirection;
}

float AAlienShip::GetFireRate()
{
	return m_fireRate;
}

void AAlienShip::SetFireRate(float fireRateSeconds)
{
	m_fireRate = fireRateSeconds;
}

int AAlienShip::GetRewardScore()
{
	return m_rewardScore;
}

void AAlienShip::SetRewardScore(int score)
{
	m_rewardScore = score;
}

void AAlienShip::Move(float deltaTime)
{
	if (!m_bIsAlive)
		return;

	m_location = GetActorLocation();
	if (m_location.X < -m_playArea.X)
	{
		m_location.X = m_playArea.X;
		m_bEdgeOfWorld = true;
	}
	if (m_location.X > m_playArea.X)
	{
		m_location.X = -m_playArea.X;
		m_bEdgeOfWorld = true;
	}

	if (m_location.Y < -m_playArea.Y)
	{
		m_location.Y = m_playArea.Y;
		m_bEdgeOfWorld = true;
	}
	if (m_location.Y > m_playArea.Y)
	{
		m_location.Y = -m_playArea.Y;
		m_bEdgeOfWorld = true;
	}

	SetActorLocation((m_location + m_movementDirection * deltaTime), !m_bEdgeOfWorld);

	if (m_bEdgeOfWorld)
		m_bEdgeOfWorld = false;
}

void AAlienShip::FireAtPlayer()
{
	if (!m_bCanFire)
		return;

	m_bCanFire = false;

	UWorld* world = GetWorld();
	FVector location = GetActorLocation();
	FRotator rotation = GetActorRotation();

	// Spawn projectile & add offset
	AAlienShipProjectile* projectile = (AAlienShipProjectile*)world->SpawnActor<AAlienShipProjectile>(AAlienShipProjectile::StaticClass(), location, rotation);
	projectile->AddActorLocalOffset(FVector(100.0f, 0.0f, 0.0f));

	// Find direction vector to player
	FRotator rotat = UKismetMathLibrary::FindLookAtRotation(m_player->GetActorLocation(), GetActorLocation());
	FVector vec = rotat.Vector();
	vec.Normalize();

	// Invert vector and shoot to target
	projectile->SetMovementDirection(-vec * m_projectileSpeed);

	// Play shoot sound
	if (m_fireSound)
		UGameplayStatics::PlaySoundAtLocation(this, m_fireSound, GetActorLocation());
}

