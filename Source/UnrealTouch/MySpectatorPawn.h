// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "MySpectatorPawn.generated.h"

/**
*
*/
UCLASS()
class UNREALTOUCH_API AMySpectatorPawn : public ASpectatorPawn
{
	GENERATED_BODY()


public:
	// Sets default values for this character's properties
	AMySpectatorPawn();
	//bla
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent);

	struct Finger1
	{
		Finger1() { bIsPressed = false; Location = FVector::ZeroVector; bool bSpecial = false; }
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
		bool bSpecial;
	};
	struct Finger2
	{
		Finger2() { bIsPressed = false; Location = FVector::ZeroVector; }
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	Finger1 TouchItem;
	Finger2 TouchItem2;
	FVector PriorDelta = FVector::ZeroVector;

	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);

	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);

	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);

};


