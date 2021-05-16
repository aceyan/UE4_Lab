// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UE4LabPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UE4LAB_API AUE4LabPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	virtual bool InputTouch(uint32 Handle, ETouchType::Type Type, const FVector2D& TouchLocation, FDateTime DeviceTimestamp, uint32 TouchpadIndex) override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Category = "TouchInput"))
		float SwipeSpeed = 0.1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Category = "TouchInput"))
		float DistanceThreshold = 12;

	FVector2D TouchStart;
	FVector2D TouchPrior;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (Category = "TouchInput"))
		bool IsTouching = false;
};
