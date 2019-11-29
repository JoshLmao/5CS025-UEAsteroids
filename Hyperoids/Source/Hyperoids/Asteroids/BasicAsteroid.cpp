// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicAsteroid.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

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

	if (m_asteroidMesh != NULL)
		asteroidComponent->SetStaticMesh(m_asteroidMesh);
	else 
		UE_LOG(LogTemp, Warning, TEXT("No Mesh set for BasicAsteroid. Won't have any"));

	OnActorBeginOverlap.AddDynamic(this, &ABasicAsteroid::OnOverlap);
	OnActorEndOverlap.AddDynamic(this, &ABasicAsteroid::OnEndOverlap);
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

}

void ABasicAsteroid::OnOverlap(AActor* overlappedActor, AActor* otherActor)
{
	
}

void ABasicAsteroid::OnEndOverlap(AActor* overlappedActor, AActor* otherActor)
{

}