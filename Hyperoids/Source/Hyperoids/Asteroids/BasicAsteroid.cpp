// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicAsteroid.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

const FName ABasicAsteroid::ASTEROID_TAG = FName("asteroid");

// Sets default values
ABasicAsteroid::ABasicAsteroid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UCapsuleComponent* capsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RootComponent"));

	RootComponent = capsuleComponent;

	capsuleComponent->SetCapsuleRadius(100.0f);
	capsuleComponent->SetCapsuleHalfHeight(150.0f);
	capsuleComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	UStaticMeshComponent* asteroidComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AsteroidMesh"));
	asteroidComponent->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh> AsteroidMesh(TEXT("StaticMesh'/Game/ExampleContent/Landscapes/Meshes/SM_Rock.SM_Rock'"));
	if (AsteroidMesh.Succeeded())
		asteroidComponent->SetStaticMesh(AsteroidMesh.Object);
	else 
		UE_LOG(LogTemp, Warning, TEXT("No Mesh set for BasicAsteroid. Won't have any"));

	OnActorBeginOverlap.AddDynamic(this, &ABasicAsteroid::OnOverlap);
	OnActorEndOverlap.AddDynamic(this, &ABasicAsteroid::OnEndOverlap);

	m_movementDirection = FVector(FMath::RandRange(-400.0f, 400.0f), FMath::RandRange(-400.0f, 400.0f), 0.0f);
	m_rotationAmount = FMath::RandRange(-350.0f, 450.0f);

	// Set the tag for all asteroids as a way to identify collisions
	this->Tags.Add(ASTEROID_TAG);
}

// Called when the game starts or when spawned
void ABasicAsteroid::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorLocation(GetRndVectorInBoundary(1000.0f, 1000.0f));
}

// Called every frame
void ABasicAsteroid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_location = GetActorLocation();

	float maxX = 1000.0f;
	float maxY = 1900.0f;
	if (m_location.X < -maxX)
	{
		m_location.X = maxX;
		bEdgeOfWorld = true;
	}
	if (m_location.X > maxX)
	{
		m_location.X = -maxX;
		bEdgeOfWorld = true;
	}

	if (m_location.Y < -maxY)
	{
		m_location.Y = maxY;
		bEdgeOfWorld = true;
	}
	if (m_location.Y > maxY)
	{
		m_location.Y = -maxY;
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
	UE_LOG(LogTemp, Log, TEXT("Tag: %d Name: %s"), overlappedActor->Tags.Num(), *overlappedActor->GetName());
}

void ABasicAsteroid::OnEndOverlap(AActor* overlappedActor, AActor* otherActor)
{

}

FVector ABasicAsteroid::GetRndVectorInBoundary(float maxX, float maxY)
{
	FVector position;
	position.X = FMath::RandRange(-maxX, maxX);
	position.Y = FMath::RandRange(-maxY, maxY);
	position.Z = 0.0f;
	return position;
}