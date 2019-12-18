// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"

#include "HyperoidsGameModeBase.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OnActorBeginOverlap.AddDynamic(this, &AProjectileBase::OnOverlap);
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
	AHyperoidsGameModeBase* gm = (AHyperoidsGameModeBase*)GetWorld()->GetAuthGameMode();
	m_playArea = gm->GetPlayArea();
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
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

void AProjectileBase::OnOverlap(AActor* overlappedActor, AActor* otherActor)
{
	// Can be overridden to be controlled
}

void AProjectileBase::SetMovementDirection(FVector moveDir)
{
	m_movementDirection = moveDir;
}