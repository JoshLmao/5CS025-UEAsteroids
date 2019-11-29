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
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("StaticMesh'/Game/ExampleContent/Input_Examples/Meshes/SM_UFO_Main.SM_UFO_Main'"));
	if (ShipMesh.Succeeded())
	{
		m_shipMeshComponent->SetStaticMesh(ShipMesh.Object);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Mesh has been set for the player!"));
	}

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

	const FVector LocalMove = FVector(m_currentForwardSpeed * DeltaTime, 0.f, 0.f);
	AddActorLocalOffset(LocalMove, true);

	FRotator DeltaRotation(0, 0, 0);
	DeltaRotation.Yaw = m_currentRotationSpeed * DeltaTime;
	AddActorLocalRotation(DeltaRotation);
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