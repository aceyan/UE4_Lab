// Fill out your copyright notice in the Description page of Project Settings.

#include "APaintable.h"
#include "TextureResource.h"



float AAPaintable::UpdateCoveragePct()
{
	if (CanvasRenderTarget != NULL)
	{
		FTextureRenderTarget2DResource*  TextureResource = (FTextureRenderTarget2DResource*)CanvasRenderTarget->Resource;
		TArray<FFloat16Color> ColorBuffer16;
		if (TextureResource->ReadFloat16Pixels(ColorBuffer16))
		{
			UE_LOG(LogTemp, Warning, TEXT("ReadFloat16Pixels success"));

			int coverageNum = 0;
			for (int i = 0; i < ColorBuffer16.Num(); i++)
			{
				FFloat16 r = ColorBuffer16[i].R;
				if (r > 0.5)
				{
					coverageNum++;
				}
			}
			CacheCoveragePct = (float)coverageNum / ColorBuffer16.Num();
			return CacheCoveragePct;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ReadFloat16Pixels fails"));
			return 0;
		}
	}
	return 0;

}