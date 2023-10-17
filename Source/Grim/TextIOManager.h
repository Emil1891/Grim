// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TextIOManager.generated.h"

/**
 * https://forums.unrealengine.com/t/how-can-i-load-text-from-file-with-blueprint/296750/2
 */
UCLASS()
class GRIM_API UTextIOManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public: 
	UFUNCTION(BlueprintCallable, Category = "save")
	static bool FileSaveString(FString SaveText);

	UFUNCTION(BlueprintPure, Category = "save")
	static bool FileLoadString(FString& SaveTextOut);

	static FString GetToken(const int TokenNumber);

	enum ETokens 
	{
		SoundPropToggle = 1 
	};
	
private:

	inline static FString FileName = "LogParameters.txt";

};
