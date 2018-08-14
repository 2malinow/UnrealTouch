// Fill out your copyright notice in the Description page of Project Settings.

#include "MySpectatorPawn.h"

#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Engine.h"


// Sets default values
AMySpectatorPawn::AMySpectatorPawn()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMySpectatorPawn::BeginPlay()
{
	Super::BeginPlay();

}

void AMySpectatorPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AMySpectatorPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	EnableTouchscreenMovement(PlayerInputComponent);
}

bool AMySpectatorPawn::EnableTouchscreenMovement(UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AMySpectatorPawn::EndTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AMySpectatorPawn::TouchUpdate);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AMySpectatorPawn::BeginTouch);
		return true;
	}

	return false;
}

void AMySpectatorPawn::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{

	if (TouchItem.bIsPressed && TouchItem2.bIsPressed)
	{
		return;
	}
	if (TouchItem.bIsPressed && !TouchItem2.bIsPressed)
	{

		TouchItem2.bIsPressed = true;
		TouchItem2.FingerIndex = FingerIndex;
		TouchItem2.Location = Location;
		TouchItem2.bMoved = false;
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Green, TEXT("Touch2 wurde gedrueckt"));
		FString x = Location.ToString();
		GEngine->AddOnScreenDebugMessage(2, 1.0f, FColor::Red, x);
		return;
	}

	GEngine->AddOnScreenDebugMessage(3, 1.0f, FColor::Green, TEXT("Touch1 wurde gedrueckt"));

	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
	FString x = Location.ToString();
	GEngine->AddOnScreenDebugMessage(4, 1.0f, FColor::Red, x);
}


void AMySpectatorPawn::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (!TouchItem.bIsPressed && !TouchItem2.bIsPressed)
	{
		return;
	}
	if (!TouchItem.bIsPressed && TouchItem2.bIsPressed)
	{
		GEngine->AddOnScreenDebugMessage(5, 1.0f, FColor::Green, TEXT("Touch2 wurde losgelassen"));
		TouchItem2.bIsPressed = false;
		//TouchItem2.Location = FVector::ZeroVector;
		return;
	}
	GEngine->AddOnScreenDebugMessage(6, 1.0f, FColor::Green, TEXT("Touch1 wurde losgelassen"));
	TouchItem.bIsPressed = false;
	TouchItem.bSpecial = true;

}

void AMySpectatorPawn::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if ((TouchItem.bIsPressed == true && TouchItem2.bIsPressed == true) &&
		(TouchItem.FingerIndex == FingerIndex || TouchItem2.FingerIndex == FingerIndex))
	{
		if (GetWorld() != nullptr)
		{
			UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
			if (ViewportClient != nullptr)
			{
				if (TouchItem.FingerIndex == FingerIndex)
				{
					TouchItem.Location = Location;
				}
				else
				{
					TouchItem2.Location = Location;
				}
				FVector MoveDelta = TouchItem2.Location - TouchItem.Location;

				FVector2D ScreenSize;
				ViewportClient->GetViewportSize(ScreenSize);
				FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
				FVector2D PriorScaledDelta = FVector2D(PriorDelta.X, PriorDelta.Y) / ScreenSize;

				float newValue = sqrt((ScaledDelta.X*ScaledDelta.X) + (ScaledDelta.Y*ScaledDelta.Y));
				float PriorAbsolute = sqrt((PriorScaledDelta.X*PriorScaledDelta.X) + (PriorScaledDelta.Y * PriorScaledDelta.Y));
				GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Red, FString::Printf(TEXT("Movedelta: %f PriorDelta:  %f"), newValue, PriorAbsolute));

				if (newValue > PriorAbsolute)
				{
					MoveForward(newValue * 100);

				}
				if (newValue < PriorAbsolute)
				{
					MoveForward(-newValue * 100);

				}
				else
				{

				}
				PriorDelta = MoveDelta;

			}

		}

	}
	if ((TouchItem.bIsPressed) && (TouchItem.FingerIndex == FingerIndex) &&
		(!TouchItem2.bIsPressed))
	{
		if (GetWorld() != nullptr)
		{
			UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
			if (ViewportClient != nullptr)
			{
				FVector MoveDelta = Location - TouchItem.Location;
				FVector2D ScreenSize;
				ViewportClient->GetViewportSize(ScreenSize);
				FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
				if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
				{
					TouchItem.bMoved = true;
					float Value = ScaledDelta.X * BaseTurnRate;
					AddControllerYawInput(Value);
				}
				if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
				{
					TouchItem.bMoved = true;
					float Value = ScaledDelta.Y * BaseTurnRate;
					AddControllerPitchInput(Value);
				}
				TouchItem.Location = Location;
			}
			TouchItem.Location = Location;
		}
	}
	if ((!TouchItem.bIsPressed) && ((TouchItem2.FingerIndex == FingerIndex) || (TouchItem.FingerIndex == FingerIndex)) &&
		(TouchItem2.bIsPressed))
	{
		if (GetWorld() != nullptr)
		{
			UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
			if (ViewportClient != nullptr)
			{

				FVector MoveDelta = Location - TouchItem2.Location;
				FVector2D ScreenSize;
				ViewportClient->GetViewportSize(ScreenSize);
				FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
				if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
				{
					if (TouchItem.bSpecial)
					{
						TouchItem2.bMoved = true;
						float Value = ScaledDelta.X * BaseTurnRate;
						AddControllerYawInput(Value);
						AddControllerYawInput(-Value);
					}
					else
					{
						TouchItem2.bMoved = true;
						float Value = ScaledDelta.X * BaseTurnRate;
						AddControllerYawInput(Value);
					}
				}
				if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
				{
					if (TouchItem.bSpecial)
					{
						TouchItem2.bMoved = true;
						float Value = ScaledDelta.Y * BaseTurnRate;
						AddControllerPitchInput(Value);
						AddControllerPitchInput(-Value);
					}
					else
					{
						TouchItem2.bMoved = true;
						float Value = ScaledDelta.Y * BaseTurnRate;
						AddControllerPitchInput(Value);
					}
				}
				TouchItem2.Location = Location;
				TouchItem.bSpecial = false;
			}
			TouchItem2.Location = Location;
		}
	}

}
