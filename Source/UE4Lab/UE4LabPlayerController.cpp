// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4LabPlayerController.h"
#include "Engine.h"


bool AUE4LabPlayerController::InputTouch(uint32 Handle, ETouchType::Type Type, const FVector2D& TouchLocation, FDateTime DeviceTimestamp, uint32 TouchpadIndex)
{
	
	APlayerController::InputTouch(Handle, Type, TouchLocation, DeviceTimestamp, TouchpadIndex);
	if (Type == ETouchType::Began)
	{
		UE_LOG(LogTemp, Warning, TEXT("Touch Began"));

		const FVector2D viewPortSize = GEngine->GameViewport->Viewport->GetSizeXY();
		FVector2D a = FVector2D(1, 1);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("size: %s"), *viewPortSize.ToString()));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("TouchLocation: %s"), *TouchLocation.ToString()));
		
		TouchStart = TouchLocation;
		TouchPrior = TouchLocation;
		
		IsTouching = true;
	}
	else if (Type == ETouchType::Ended)
	{
		UE_LOG(LogTemp, Warning, TEXT("Touch Ended"));
		TouchStart = FVector2D::ZeroVector;
		TouchPrior = FVector2D::ZeroVector;
		IsTouching = false;
	}
	else if (Type == ETouchType::Moved)
	{
		UE_LOG(LogTemp, Warning, TEXT("Touch Moved"));

		if (TouchStart.Size() == 0 && TouchPrior.Size() == 0)
		{
			TouchStart = TouchLocation;
			TouchPrior = TouchLocation;
			IsTouching = true;
		}

		float length = FVector2D(TouchLocation - TouchStart).Size();

		if (length >= DistanceThreshold)
		{

			FVector2D dist = FVector2D(TouchLocation - TouchPrior) * SwipeSpeed;

			UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->AddControllerYawInput(dist.X);
			UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->AddControllerPitchInput(dist.Y);

			TouchPrior = TouchLocation;
		}
	}

	return true;
}
