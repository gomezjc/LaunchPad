// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "LaunchPadGameMode.h"
#include "LaunchPadCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALaunchPadGameMode::ALaunchPadGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
