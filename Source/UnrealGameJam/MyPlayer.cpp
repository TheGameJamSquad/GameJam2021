// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer.h"



#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/ArrowComponent.h"
// Sets default values
AMyPlayer::AMyPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(RootComponent);
	
	HeldItemLocation = CreateDefaultSubobject<USceneComponent>("Held Item Location");
	HeldItemLocation->SetupAttachment(Camera);

	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>("Physics Handle");
}

// Called when the game starts or when spawned
void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();
	ShowPaperDocument();
}

// Called every frame
void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(PhysicsHandle)
	{
		UPrimitiveComponent* Component = PhysicsHandle->GrabbedComponent;
		if(Component)
		{
			PhysicsHandle->SetTargetLocation(HeldItemLocation->GetComponentLocation());
		}
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
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMyPlayer::ToggleCrouch);
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
	if (!PhysicsHandle) { return; }
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
				PhysicsHandle->GrabComponentAtLocationWithRotation(Component, NAME_None, Component->GetComponentLocation(), FRotator::ZeroRotator);
			}
		}	
	}
}

void AMyPlayer::Grab(AActor* ActorToGrab)
{
	UPrimitiveComponent* Component = ActorToGrab->FindComponentByClass<UPrimitiveComponent>();
	if(PhysicsHandle && Component && Component->IsSimulatingPhysics())
	{
		PhysicsHandle->GrabComponentAtLocation(Component, NAME_None, Component->GetComponentLocation());
	}
}

void AMyPlayer::ToggleCrouch()
{
	FVector Location = Camera->GetRelativeLocation();
	if(bIsCrouching)
	{
		Location.Z += 50.0f;
	}
	else
	{
		Location.Z -= 50.0f;
	}
	Camera->SetRelativeLocation(Location);
	bIsCrouching = !bIsCrouching;
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

AActor* AMyPlayer::GetHandledActor() const
{
	if(!PhysicsHandle) { return nullptr; }
	if(PhysicsHandle->GrabbedComponent)
	{
		return PhysicsHandle->GrabbedComponent->GetOwner();
	}
	
	return nullptr;
}
