// Fill out your copyright notice in the Description page of Project Settings.


#include "AudioPlayTimes.h"
#include "Components/AudioComponent.h"

// Sets default values for this component's properties
UAudioPlayTimes::UAudioPlayTimes()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}

void UAudioPlayTimes::SetPlayTimes(TArray<UAudioComponent*>& AudioComponents)
{
	for(auto AudioComp : AudioComponents)
	{
		// Bind event to call when sound play time changes 
		AudioComp->OnAudioPlaybackPercent.AddDynamic(this, &UAudioPlayTimes::OnPlayBackChanged);
		AudioComp->Play(); // Needs to call play for some reason for it to work 
		
		PlayTimes.Add(AudioComp); 
	}
}

float UAudioPlayTimes::GetPlayTime(const UAudioComponent* AudioComp) const
{
	if(PlayTimes.Contains(AudioComp)) 
		return PlayTimes[AudioComp];

	return -1; 
}

void UAudioPlayTimes::OnPlayBackChanged(const USoundWave* PlayingSoundWave, const float PlayBackPercent)
{
	// Since the event only passes the sound playing and not the audio component, we first have to find the audio comp
	// playing this sound. This causes a problem when multiple audio comps are playing the same sound 
	for(const auto [AudioComp, PlayTime] : PlayTimes)
	{
		if(AudioComp->GetSound() == PlayingSoundWave)
		{
			// Sounds that loop get percentage of over 1 so need to get rid of the integer part of the number 
			const float RealPlaybackPercent = FMath::Fmod(PlayBackPercent, 1); 
			UE_LOG(LogTemp, Warning, TEXT("Play time: %f"), RealPlaybackPercent * PlayingSoundWave->Duration)
			PlayTimes[AudioComp] = RealPlaybackPercent * PlayingSoundWave->Duration;
			
			return; 
		}
	}
}



