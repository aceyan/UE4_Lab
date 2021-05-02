// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4LabPlayerController.h"
#include "Engine.h"


bool AUE4LabPlayerController::InputTouch(uint32 Handle, ETouchType::Type Type, const FVector2D& TouchLocation, FDateTime DeviceTimestamp, uint32 TouchpadIndex)
{
	

	if (Type == ETouchType::Began)
	{
		UE_LOG(LogTemp, Warning, TEXT("Touch Began"));

		const FVector2D viewPortSize = GEngine->GameViewport->Viewport->GetSizeXY();
		if (TouchLocation.X >= viewPortSize.X / 2.0)
		{
			TouchStart = TouchLocation;
			TouchPrior = TouchLocation;
		}

	}
	else if (Type == ETouchType::Ended)
	{
		UE_LOG(LogTemp, Warning, TEXT("Touch Ended"));
	}
	else if (Type == ETouchType::Moved)
	{
		UE_LOG(LogTemp, Warning, TEXT("Touch Moved"));
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
