// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownCameraController.h"

#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "GameFramework\PlayerController.h"

// Sets default values
ATopDownCameraController::ATopDownCameraController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Top Down Camera"));
	RootComponent = m_cameraComponent;
}

// Called when the game starts or when spawned
void ATopDownCameraController::BeginPlay()
{
	Super::BeginPlay();

	UWorld* world = GetWorld();
	if (world != NULL)
	{
		APlayerController* playerController = world->GetFirstPlayerController();
		if (playerController)
			playerController->SetViewTarget(this);
	}
	
}

// Called every frame
void ATopDownCameraController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

