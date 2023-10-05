// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableAudioPlayer.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

AInteractableAudioPlayer::AInteractableAudioPlayer()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	TriggerZone->SetupAttachment(Mesh);

	AudioPlayer = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioPlayer"));
	AudioPlayer->SetupAttachment(Mesh);
}

void AInteractableAudioPlayer::InteractSuccessful()
{
	// So interacting again does not reset audio or fire event 
	if(AudioPlayer->IsPlaying())
		return; 
	
	Super::InteractSuccessful();

	AudioPlayer->Play(); 
}
