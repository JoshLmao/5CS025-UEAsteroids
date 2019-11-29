// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceshipPawn.h"

#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/CollisionProfile.h"

// Sets default values
ASpaceshipPawn::ASpaceshipPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_shipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ship Mesh"));
	m_shipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	RootComponent = m_shipMeshComponent;
	
	// Set model of player
	if (m_shipModel != NULL)
		m_shipMeshComponent->SetStaticMesh(m_shipModel);
	else
		UE_LOG(LogTemp, Warning, TEXT("No Mesh has been set for the player!"));
	
	m_currentForwardSpeed = 0.0f;
	m_currentRotationSpeed = 0.0f;
	m_currentFire = 0.0f;

	m_bCanFire = true;
	m_fireRate = 1.0f;
	m_gunOffset = 70.0f;

	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("SoundWave'/Game/TwinStick/Audio/TwinStickFire.TwinStickFire'"));
	if (FireAudio.Succeeded())
		m_fireSound = FireAudio.Object;
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

	const FVector local = FVector(m_currentForwardSpeed * DeltaTime, 0.0f, 0.0f);
	AddActorLocalOffset(local, true);

	FRotator deltaRotation(0, 0, 0);
	deltaRotation.Yaw = m_currentRotationSpeed * DeltaTime;
	AddActorLocalRotation(deltaRotation);
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
		m_currentForwardSpeed = value * 250.0f;
	else
		m_currentForwardSpeed = 0.0f;
}

void ASpaceshipPawn::MoveRightInput(float value)
{
	if (!FMath::IsNearlyEqual(value, 0.0f))
		m_currentRotationSpeed = value * 250.0f;
	else
		m_currentRotationSpeed = 0.0f;
}

void ASpaceshipPawn::FireInput(float value)
{
	if (FMath::IsNearlyEqual(value, 0.0f))
		m_currentFire = value;
	else
		m_currentFire = 0.0f;
}

void ASpaceshipPawn::ShotTimerExpired()
{
	m_bCanFire = true;
}