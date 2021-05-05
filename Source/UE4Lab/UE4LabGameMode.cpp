// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "UE4LabGameMode.h"
#include "UE4LabCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUE4LabGameMode::AUE4LabGameMode()
{
	// set default pawn class to our Blueprinted character
	/*static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/BP_UE4LabCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}*/
}
