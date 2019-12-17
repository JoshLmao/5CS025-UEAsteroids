// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicAsteroid.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

#include "HyperoidsGameModeBase.h"
#include "Player\ShipProjectile.h"

const FName ABasicAsteroid::ASTEROID_TAG = FName("asteroid");

// Sets default values
ABasicAsteroid::ABasicAsteroid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_bIsChildAsteroid = false;

	UCapsuleComponent* capsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RootComponent"));
	RootComponent = capsuleComponent;
	// Configure collider
	m_colliderSize = 30.0f;
	capsuleComponent->SetCapsuleRadius(m_colliderSize);
	capsuleComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	UStaticMeshComponent* asteroidComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AsteroidMesh"));
	asteroidComponent->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh> AsteroidMesh(TEXT("StaticMesh'/Game/ExampleContent/Landscapes/Meshes/SM_Rock.SM_Rock'"));
	if (AsteroidMesh.Succeeded())
	{
		asteroidComponent->SetStaticMesh(AsteroidMesh.Object);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Mesh set for BasicAsteroid!"));
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> asteroidExplodeSound(TEXT("SoundWave'/Game/Sounds/meteor_explode.meteor_explode'"));
	if (asteroidExplodeSound.Succeeded())
		m_explodeSound = asteroidExplodeSound.Object;

	OnActorBeginOverlap.AddDynamic(this, &ABasicAsteroid::OnOverlap);

	m_rewardScore = 10;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionSystem(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	if (ExplosionSystem.Succeeded())
		m_explodePS = ExplosionSystem.Object;
}

// Called when the game starts or when spawned
void ABasicAsteroid::BeginPlay()
{
	Super::BeginPlay();

	AHyperoidsGameModeBase* gm = (AHyperoidsGameModeBase*)GetWorld()->GetAuthGameMode();
	m_playArea = gm->GetPlayArea();
}

// Called every frame
void ABasicAsteroid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_location = GetActorLocation();
	if (m_location.X < -m_playArea.X)
	{
		m_location.X = m_playArea.X;
		bEdgeOfWorld = true;
	}
	if (m_location.X > m_playArea.X)
	{
		m_location.X = -m_playArea.X;
		bEdgeOfWorld = true;
	}

	if (m_location.Y < -m_playArea.Y)
	{
		m_location.Y = m_playArea.Y;
		bEdgeOfWorld = true;
	}
	if (m_location.Y > m_playArea.Y)
	{
		m_location.Y = -m_playArea.Y;
		bEdgeOfWorld = true;
	}
	
	SetActorLocation((m_location + m_movementDirection * DeltaTime), !bEdgeOfWorld);

	if (bEdgeOfWorld)
		bEdgeOfWorld = false;

	FRotator actorRotation = GetActorRotation();
	actorRotation.Roll += m_rotationAmount * DeltaTime;

	SetActorRotation(actorRotation);
}

void ABasicAsteroid::OnOverlap(AActor* overlappedActor, AActor* otherActor)
{
	if (otherActor->IsA(AShipProjectile::StaticClass()))
	{
		// Spawn Asteroids if not a child asteroid
		if (!m_bIsChildAsteroid)
		{
			int childAsteroidAmt = FMath::RandRange(1, 3);
			SpawnChildAsteroids(childAsteroidAmt);
			UE_LOG(LogTemp, Log, TEXT("Created '%d' child asteroids"), childAsteroidAmt);
		}

		// Play Explosion sound for Asteroid
		if (m_explodeSound) {
			UGameplayStatics::PlaySoundAtLocation(this, m_explodeSound, GetActorLocation());
		}

		// Display Explosion in place of player's ship
		if (m_explodePS) {
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_explodePS, this->GetTransform());
		}

		// Call event to show asteroid has been destroyed
		if (OnAsteroidDestroyed.IsBound())
			OnAsteroidDestroyed.Broadcast(this);

		// Finally destroy once done
		Destroy();
	}
}

void ABasicAsteroid::SetColliderSize(float size)
{
	m_colliderSize = size;

	UCapsuleComponent* capsule = (UCapsuleComponent*)GetComponentByClass(UCapsuleComponent::StaticClass());
	//capsule->SetCapsuleHalfHeight(m_colliderSize);
	capsule->SetCapsuleRadius(m_colliderSize);
}

void ABasicAsteroid::SpawnChildAsteroids(int amount)
{
	UWorld* world = GetWorld();
	
	float degreeSegment = 120.0f;
	const FVector location = GetActorLocation();
	const FRotator rotation = GetActorRotation();
	for (int i = 0; i < amount; i++) 
	{
		ABasicAsteroid* asteroid = (ABasicAsteroid*)world->SpawnActor<ABasicAsteroid>(ABasicAsteroid::StaticClass(), location, rotation);
		asteroid->SetAsChildAsteroid();

		FVector randMovementDir = FVector(FMath::RandRange(-200.0f, 200.0f), FMath::RandRange(-200.0f, 200.0f), 0.0f);
		asteroid->SetMovementDirection(randMovementDir);

		FVector smallAsteroidScale = FVector(FMath::RandRange(1.0f, 2.0f), FMath::RandRange(1.0f, 2.0f), FMath::RandRange(1.0f, 2.0f));
		asteroid->SetActorScale3D(smallAsteroidScale);

		asteroid->SetRewardScore(m_rewardScore / amount);
	}
}

void ABasicAsteroid::SetRandomDirections()
{
	m_movementDirection = FVector(FMath::RandRange(-400.0f, 400.0f), FMath::RandRange(-400.0f, 400.0f), 0.0f);
	m_rotationAmount = FMath::RandRange(-350.0f, 450.0f);
}

FVector ABasicAsteroid::GetMovementDirection()
{
	return m_movementDirection;
}

void ABasicAsteroid::SetMovementDirection(const FVector direction)
{
	m_movementDirection = direction;
}

void ABasicAsteroid::SetAsChildAsteroid()
{
	m_bIsChildAsteroid = true;
}

FVector2D ABasicAsteroid::GetRndVectorInBoundary(FVector2D playArea)
{
	FVector2D position;
	position.X = FMath::RandRange(-playArea.X, playArea.X);
	position.Y = FMath::RandRange(-playArea.Y, playArea.Y);
	return position;
}

FVector2D ABasicAsteroid::GetRndVectorInBoundary(FVector2D playArea, FVector2D safeArea)
{
	FVector2D position;
	
	while (position.X < safeArea.X && position.X > -safeArea.X) {
		position.X = FMath::RandRange(-playArea.X, playArea.X);
	}
	
	while (position.Y > safeArea.Y && position.Y < -safeArea.Y) {
		position.Y = FMath::RandRange(-playArea.Y, playArea.Y);
	}
	
	return position;
}

int ABasicAsteroid::GetRewardScore()
{
	return m_rewardScore;
}

void ABasicAsteroid::SetRewardScore(int amount)
{
	m_rewardScore = amount;
}
