// Fill out your copyright notice in the Description page of Project Settings.


#include "TextIOManager.h"

bool UTextIOManager::FileSaveString(FString SaveText)
{
	return FFileHelper::SaveStringToFile(SaveText, *(FPaths::GameSourceDir() + FileName));
}

bool UTextIOManager::FileLoadString(FString& SaveTextOut)
{
	return FFileHelper::LoadFileToString(SaveTextOut, *(FPaths::GameSourceDir() + FileName));
}

FString UTextIOManager::GetToken(const int TokenNumber)
{
	FString FileString; 
	if(!FileLoadString(FileString))
		return FString(); 

	TArray<FString> Tokens;
	FString Delim = " "; 
	FileString.ParseIntoArray(Tokens, TEXT(" ")); 

	return Tokens[TokenNumber]; 
}
