// Copyright Epic Games, Inc. All Rights Reserved.

#include "GrimGameMode.h"
#include "GrimCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGrimGameMode::AGrimGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
