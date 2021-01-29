// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer.h"

#include "PhysicsEngine/PhysicsHandleComponent.h"

// Sets default values
AMyPlayer::AMyPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HeldItemLocation = CreateDefaultSubobject<USceneComponent>("Held Item Location");
	HeldItemLocation->SetupAttachment(RootComponent);

	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>("Physics Handle");
}

// Called when the game starts or when spawned
void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(PhysicsHandle && PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(HeldItemLocation->GetComponentLocation());
	}
}

// Called to bind functionality to input
void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyPlayer::MoveRight);
	PlayerInputComponent->BindAxis("LookHorizontal", this, &AMyPlayer::LookHorizontal);
	PlayerInputComponent->BindAxis("LookVertical", this, &AMyPlayer::LookVertical);
	PlayerInputComponent->BindAction("Grab", IE_Pressed, this, &AMyPlayer::ToggleGrab);
}

void AMyPlayer::MoveForward(float value)
{
	AddMovementInput(GetActorForwardVector(), value);
}

void AMyPlayer::MoveRight(float value)
{
	AddMovementInput(GetActorRightVector(), value);
}

void AMyPlayer::LookHorizontal(float value)
{
	AddControllerYawInput(value);
}

void AMyPlayer::LookVertical(float value)
{
	AddControllerPitchInput(-value);
}

void AMyPlayer::ToggleGrab()
{
	if ( !PhysicsHandle) { return; }
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->ReleaseComponent();
	}
	else
	{
		FHitResult HitResult;
		if(GetFirstPhysicBodyInReach(HitResult))
		{
			UPrimitiveComponent* Component = HitResult.GetComponent();
			if(Component && Component->IsSimulatingPhysics())
			{
				PhysicsHandle->GrabComponentAtLocation(Component, NAME_None, Component->GetComponentLocation());
			}
		}	
	}
}

FVector AMyPlayer::GetPlayerPosition() const
{
	FVector PlayerPosition;
	FRotator PlayerRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerPosition, PlayerRotation);
	return PlayerPosition;
}

FVector AMyPlayer::GetPlayerReach() const
{
	FVector PlayerPosition;
	FRotator PlayerRotation;
    
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerPosition, PlayerRotation);
    
	return PlayerPosition + PlayerRotation.Vector() * Reach;
}