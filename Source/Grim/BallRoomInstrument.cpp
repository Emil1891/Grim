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

	// Unreal moment where component would not show up if created in C++, workaround below where component is created
	// in the BP and then found here 
	ActiveAudioPlayer = Cast<UAudioComponent>(GetComponentsByTag(UAudioComponent::StaticClass(), FName("ActiveAudioPlayer"))[0]);
	ActiveAudioPlayer->SetSound(ActiveSound); 
	ActiveAudioPlayer->SetVolumeMultiplier(0.01f); 
	ActiveAudioPlayer->Play();
	
	// Puzzle manager assigns itself if it is not already set 
	if(!PuzzleManager)
		PuzzleManager = Cast<ABallRoomPuzzleManager>(UGameplayStatics::GetActorOfClass(this, ABallRoomPuzzleManager::StaticClass()));

	if(!ActiveSound || !IdleSound)
		UE_LOG(LogTemp, Error, TEXT("No idle or active sound!"))
}

void ABallRoomInstrument::ResetToIdleSound()
{
	// Turns on idle sound and turns off active sound by adjusting volume to keep everything in sync 
	AudioPlayer->SetVolumeMultiplier(1);
	ActiveAudioPlayer->SetVolumeMultiplier(0.01f);

	// make the instrument interactable again if player is not in the zone 
	if(!bPlayerIsInZone)
		SetIsInteractable(true);

	// If player is in the zone, it will be interactable again when they exit 
}

void ABallRoomInstrument::InteractSuccessful()
{
	// do nothing if puzzle is already complete or already playing the active sound 
	if(PuzzleManager->IsPuzzleComplete() || AudioPlayer->GetSound() == ActiveSound)
		return;
	
	Super::InteractSuccessful();

	AudioPlayer->SetVolumeMultiplier(0);
	ActiveAudioPlayer->SetVolumeMultiplier(1);

	// Turn off the interact zone sound on interact 
	InteractAudioPlayer->FadeOut(InteractFadeOutDuration, 0.01f); 

	// Turn off interactability so it cannot be interacted with when "active" 
	SetIsInteractable(false);
	
	PuzzleManager->PlayerInteractedWithInstrument(this);
}

void ABallRoomInstrument::TriggerZoneExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int OtherBodyIndex)
{
	Super::TriggerZoneExited(OverlappedComponent, OtherActor, OtherComponent, OtherBodyIndex);

	// if the instrument is not playing its active sound, then enable interaction
	// forcing the player to exit the zone if the last instrument when failing the puzzle 
	if(ActiveAudioPlayer->VolumeMultiplier < 0.1f)
	{
		SetIsInteractable(true);
		AudioPlayer->SetVolumeMultiplier(1); // Reset idle audio player on exit, depending on if player activated it or not 
	}
	else
		AudioPlayer->SetVolumeMultiplier(0.01f);
}

void ABallRoomInstrument::TriggerZoneEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::TriggerZoneEntered(OverlappedComponent, OtherActor, OtherComponent, OtherBodyIndex, bFromSweep, SweepResult);

	// Has not activated the instrument 
	if(ActiveAudioPlayer->VolumeMultiplier < 0.1f)
	{
		// Boost the volume so player can easier distinguish which instrument they will interact with 
		AudioPlayer->SetVolumeMultiplier(VolumeMultWhenInZone); 
	}
}
