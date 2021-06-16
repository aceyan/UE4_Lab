// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/TextureRenderTarget2D.h"
#include "APaintable.generated.h"


/**
 * 
 */
UCLASS()
class UE4LAB_API AAPaintable : public AStaticMeshActor
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Splat")
		float UpdateCoveragePct();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Splat")
	UTextureRenderTarget2D* CanvasRenderTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Splat")
		float CacheCoveragePct = 0;
};
