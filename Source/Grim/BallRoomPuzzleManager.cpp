// Fill out your copyright notice in the Description page of Project Settings.


#include "BallRoomPuzzleManager.h"

#include "BallRoomInstrument.h"
#include "QuestDoor.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABallRoomPuzzleManager::ABallRoomPuzzleManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ABallRoomPuzzleManager::PlayerInteractedWithInstrument(AInteractableAudioPlayer* Instrument)
{
	if(bPuzzleComplete)
		return; 
	
	// adds the instrument to the array if it does not already exist 
	PlayerInputOrder.AddUnique(Instrument);

	// if player has interacted with every instrument 
	if(PlayerInputOrder.Num() == CorrectInstrumentOrder.Num())
	{
		// if player has interacted in the correct order 
		if(PlayerInputOrder == CorrectInstrumentOrder)
		{
			UE_LOG(LogTemp, Warning, TEXT("Puzzle Complete"))
			bPuzzleComplete = true; 
			if(QuestDoor)
			    QuestDoor->QuestCompleted(); 
			else
				UE_LOG(LogTemp, Error, TEXT("No quest door is assigned to open upon puzzle complete"))
		} else
			ResetPuzzle();
	}
}

float ABallRoomPuzzleManager::GetPlayTime(const UAudioComponent* AudioComp) const
{
	if(PlayTimes.Contains(AudioComp)) 
		return PlayTimes[AudioComp];

	return -1; 
}

void ABallRoomPuzzleManager::ResetPuzzle()
{
	// reset puzzle by emptying the array 
	PlayerInputOrder.Empty();

	// and resetting every instrument to idle sound
	for(const auto Instrument : CorrectInstrumentOrder)
		if(const auto BallInstrument = Cast<ABallRoomInstrument>(Instrument))
			BallInstrument->ResetToIdleSound();
	
	if(PuzzleFailedSound)
		UGameplayStatics::PlaySound2D(this, PuzzleFailedSound); 

	UE_LOG(LogTemp, Warning, TEXT("Puzzle failed"))
}

void ABallRoomPuzzleManager::OnPlayBackChanged(const USoundWave* PlayingSoundWave, const float PlayBackPercent)
{
	// Since the event only passes the sound playing and not the audio component, we first have to find the audio comp
	// playing this sound. This causes a problem when multiple audio comps are playing the same sound 
	for(const auto [AudioComp, PlayTime] : PlayTimes)
	{
		if(AudioComp->GetSound() == PlayingSoundWave)
		{
			// Sounds that loop get percentage of over 1 so need to get rid of the integer part of the number 
			const float RealPlaybackPercent = FMath::Fmod(PlayBackPercent, 1); 
			//UE_LOG(LogTemp, Warning, TEXT("Play time: %f"), RealPlaybackPercent * PlayingSoundWave->Duration)
			PlayTimes[AudioComp] = RealPlaybackPercent * PlayingSoundWave->Duration;
			
			return; 
		}
	}
}

void ABallRoomPuzzleManager::SetPlayTimes(UAudioComponent* IdleInstrument)
{
	// Bind event to call when sound play time changes 
	IdleInstrument->OnAudioPlaybackPercent.AddDynamic(this, &ABallRoomPuzzleManager::OnPlayBackChanged);
	IdleInstrument->Play(); // Needs to call play for some reason for it to work 
	
	PlayTimes.Add(IdleInstrument);
}
