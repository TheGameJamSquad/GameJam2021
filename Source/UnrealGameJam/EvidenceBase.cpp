// Fill out your copyright notice in the Description page of Project Settings.


#include "EvidenceBase.h"

// Sets default values
AEvidenceBase::AEvidenceBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Item Mesh");
	MeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEvidenceBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEvidenceBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

