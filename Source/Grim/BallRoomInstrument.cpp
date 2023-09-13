// Fill out your copyright notice in the Description page of Project Settings.


#include "BallRoomInstrument.h"

#include "BallRoomPuzzleManager.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

void ABallRoomInstrument::BeginPlay()
{
	Super::BeginPlay();

	AudioPlayer->SetSound(IdleSound);
	AudioPlayer->Play();

	// create the active audio player, start playing it but turn volume off.
	// playing all audio from the start and only adjusting the volume ensures perfect sync 
	// ActiveAudioPlayer = CreateDefaultSubobject<UAudioComponent>(TEXT("ActiveAudioPlayer"));
	// ActiveAudioPlayer->SetupAttachment(RootComponent);

	ActiveAudioPlayer = Cast<UAudioComponent>(GetComponentsByTag(UAudioComponent::StaticClass(), FName("ActiveAudioPlayer"))[0]);  
	ActiveAudioPlayer->Play();
	ActiveAudioPlayer->VolumeMultiplier = 0; 
	
	// Puzzle manager assigns itself if it is not already set 
	if(!PuzzleManager)
		PuzzleManager = Cast<ABallRoomPuzzleManager>(UGameplayStatics::GetActorOfClass(this, ABallRoomPuzzleManager::StaticClass()));

	if(!ActiveSound || !IdleSound)
		UE_LOG(LogTemp, Error, TEXT("No idle or active sound!"))
}

void ABallRoomInstrument::ChangeToIdleSound()
{
	// AudioPlayer->SetSound(IdleSound);

	// Turns on idle sound and turns off active sound by adjusting volume to keep everything in sync 
	AudioPlayer->VolumeMultiplier = 1;
	ActiveAudioPlayer->VolumeMultiplier = 0; 
}

void ABallRoomInstrument::InteractSuccessful()
{
	// do nothing if puzzle is already complete or already playing the active sound 
	if(PuzzleManager->IsPuzzleComplete() || AudioPlayer->GetSound() == ActiveSound)
		return;
	
	Super::InteractSuccessful();
	
	//AudioPlayer->SetSound(ActiveSound);

	// change volume on both audio players, turning on the active one 
	// only adjusting the volume will keep the various audio in sync 
	AudioPlayer->VolumeMultiplier = 0;
	ActiveAudioPlayer->VolumeMultiplier = 1; 
	
	PuzzleManager->PlayerInteractedWithInstrument(this);
}
