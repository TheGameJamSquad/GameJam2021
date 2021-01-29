// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


class UPhysicsHandleComponent;
class USceneComponent;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALGAMEJAM_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:

	UGrabber();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	// Called when the game starts
	virtual void BeginPlay() override;
	void InitializePhysicsHandle();
	void InitializeInput();
	void Grab();
	void Release();
	FHitResult GetFirstPhysicBodyInReach() const;
	FVector GetPlayerPosition() const;
	FVector GetPlayerReach() const;
	FVector GetPlayerCenter() const;

private:
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;
	APlayerController* PlayerController = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ItemHeldPosition = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Range", meta = (AllowPrivateAccess = "true"))
	float Reach = 100.f;
};
