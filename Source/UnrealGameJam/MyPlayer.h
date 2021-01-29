// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyPlayer.generated.h"

class UPhysicsHandleComponent;
UCLASS()
class UNREALGAMEJAM_API AMyPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyPlayer();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);
	void MoveRight(float value);
	void LookHorizontal(float value);
	void LookVertical(float value);
	void ToggleGrab();
	void Release();

	UFUNCTION(BlueprintImplementableEvent)
	bool GetFirstPhysicBodyInReach(FHitResult& hit) const;

	UFUNCTION(BlueprintCallable)
	FVector GetPlayerPosition() const;
	
	UFUNCTION(BlueprintCallable)
	FVector GetPlayerReach() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* HeldItemLocation = nullptr;
	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	UPROPERTY(EditAnywhere)
	float Reach = 300.0f;
};
