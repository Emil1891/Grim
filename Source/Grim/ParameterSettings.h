// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "ParameterSettings.generated.h"

/**
 * 
 */
UCLASS()
class GRIM_API UParameterSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	static UParameterSettings* GetParamSettings(); 
	
	UPROPERTY(Config, BlueprintReadOnly)
	bool SoundPropEnable = true; 

	UPROPERTY(Config, BlueprintReadOnly)
	bool PlayImmersiveSoundFile = true; 
	
};
