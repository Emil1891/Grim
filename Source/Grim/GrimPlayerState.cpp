// Fill out your copyright notice in the Description page of Project Settings.

#include "GrimPlayerState.h"

#include "FearPoint.h"
#include "Components/AudioComponent.h"
#include "Components/ForceFeedbackComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

AGrimPlayerState::AGrimPlayerState()
{
	PrimaryActorTick.bCanEverTick = true; 
}

void AGrimPlayerState::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> TempArray; 
	UGameplayStatics::GetAllActorsOfClass(this, AFearPoint::StaticClass(), TempArray);

	for(auto FearPoint : TempArray)
		AllFearPoints.Add(Cast<AFearPoint>(FearPoint));

	// set up the audio component handling fear level and the heartbeat 
	FearAudioComponent = UGameplayStatics::CreateSound2D(this, FearSoundCue);
	FearAudioComponent->SetFloatParameter(FName("Proximity"), 1);
	FearAudioComponent->Play();

	if(ForceFeedbackEffect)
	{
		ForceFeedbackComponent = UGameplayStatics::SpawnForceFeedbackAttached(ForceFeedbackEffect, GetPlayerController()->GetCharacter()->GetRootComponent(), NAME_None, FVector(0), FRotator(0), EAttachLocation::KeepRelativeOffset, false, true, 1, 0);
		ForceFeedbackEffect->Duration = VibrationDuration;
	}
}

void AGrimPlayerState::SetFearLevel()
{
	// Finds the highest fear level (nearer to 1 means more fear)
	float MaxFear = 0;
	
	for(auto FearPoint : AllFearPoints)
	{
		const float FearPointFearLevel = FearPoint->GetFearLevel(); 
		if(FearPointFearLevel > MaxFear)
			MaxFear = FearPointFearLevel; 
	}
	FearLevel = MaxFear;
}

void AGrimPlayerState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	SetFearLevel();
	
	// update the fear/proximity value in the audio component 
	FearAudioComponent->SetFloatParameter(FName("Proximity"), FearLevel);

	ForceFeedbackComponent->IntensityMultiplier = std::sqrt(FearLevel)/2 - .27f;
}
