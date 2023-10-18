// Fill out your copyright notice in the Description page of Project Settings.


#include "ParameterSettings.h"

UParameterSettings* UParameterSettings::GetParamSettings()
{
	return Cast<UParameterSettings>(GetGameUserSettings()); 
}
