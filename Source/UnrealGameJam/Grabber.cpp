// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	InitializePhysicsHandle();
	InitializeInput();

	PlayerController = GetWorld()->GetFirstPlayerController();
}

void UGrabber::InitializePhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("No physics handle component found on %s"), *GetOwner()->GetName());
	}
}

void UGrabber::InitializeInput()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if(InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Attemps grabbing!!"));

	FHitResult HitResult = GetFirstPhysicBodyInReach();
	UPrimitiveComponent* Component = HitResult.GetComponent();
	if(Component)
	{
		PhysicsHandle->GrabComponentAtLocation(Component, NAME_None, GetPlayerReach());
	}
}

void UGrabber::Release()
{
	if(!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}

FHitResult UGrabber::GetFirstPhysicBodyInReach() const
{
	FHitResult hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
    
	GetWorld()->LineTraceSingleByObjectType(hit, GetPlayerPosition(), GetPlayerReach(),
        FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParams);

	return hit;
}

FVector UGrabber::GetPlayerPosition() const
{
	FVector PlayerPosition;
	FRotator PlayerRotation;
	PlayerController->GetPlayerViewPoint(PlayerPosition, PlayerRotation);
	return PlayerPosition;
}

FVector UGrabber::GetPlayerReach() const
{
	FVector PlayerPosition;
	FRotator PlayerRotation;
    
	PlayerController->GetPlayerViewPoint(PlayerPosition, PlayerRotation);
    
	return PlayerPosition + PlayerRotation.Vector() * Reach;
}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
