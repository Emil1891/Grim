// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "QuestCompleteInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UQuestCompleteInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GRIM_API IQuestCompleteInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION()
	virtual void QuestCompleted() = 0;
};
