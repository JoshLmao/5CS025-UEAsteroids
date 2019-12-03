// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicAsteroid.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

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

	// Set the tag for all asteroids as a way to identify collisions
	this->Tags.Add(ASTEROID_TAG);

	OnActorBeginOverlap.AddDynamic(this, &ABasicAsteroid::OnOverlap);
	OnActorEndOverlap.AddDynamic(this, &ABasicAsteroid::OnEndOverlap);

	m_rewardScore = 10;
}

// Called when the game starts or when spawned
void ABasicAsteroid::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABasicAsteroid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_location = GetActorLocation();

	AHyperoidsGameModeBase* gm = (AHyperoidsGameModeBase*)GetWorld()->GetAuthGameMode();
	FVector2D playArea = gm->GetPlayArea();
	if (m_location.X < -playArea.X)
	{
		m_location.X = playArea.X;
		bEdgeOfWorld = true;
	}
	if (m_location.X > playArea.X)
	{
		m_location.X = -playArea.X;
		bEdgeOfWorld = true;
	}

	if (m_location.Y < -playArea.Y)
	{
		m_location.Y = playArea.Y;
		bEdgeOfWorld = true;
	}
	if (m_location.Y > playArea.Y)
	{
		m_location.Y = -playArea.Y;
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
	FName tag;
	if (otherActor->Tags.Num() > 0)
		tag = otherActor->Tags[0];

	if (tag == AShipProjectile::PROJECTILE_TAG)
	{
		// Spawn Asteroids if not a child asteroid
		if (!m_bIsChildAsteroid)
		{
			int childAsteroidAmt = FMath::RandRange(2, 4);
			SpawnChildAsteroids(childAsteroidAmt);
			UE_LOG(LogTemp, Log, TEXT("Created '%d' child asteroids"), childAsteroidAmt);
		}

		if (m_explodeSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, m_explodeSound, GetActorLocation());
		}

		auto gm = (AHyperoidsGameModeBase*)GetWorld()->GetAuthGameMode();
		gm->OnAsteroidDestroyed(this);

		// Finally destroy once done
		Destroy();
	}
}

void ABasicAsteroid::OnEndOverlap(AActor* overlappedActor, AActor* otherActor)
{

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

void ABasicAsteroid::SetMovementDirection(const FVector direction)
{
	m_movementDirection = direction;
}

void ABasicAsteroid::SetAsChildAsteroid()
{
	m_bIsChildAsteroid = true;
}

void ABasicAsteroid::SetRandomLocation()
{
	SetActorLocation(GetRndVectorInBoundary(1000.0f, 1000.0f));
}

FVector ABasicAsteroid::GetRndVectorInBoundary(float maxX, float maxY)
{
	FVector position;
	position.X = FMath::RandRange(-maxX, maxX);
	position.Y = FMath::RandRange(-maxY, maxY);
	position.Z = 0.0f;
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
