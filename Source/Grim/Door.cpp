// Fill out your copyright notice in the Description page of Project Settings.

#include "Door.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

ADoor::ADoor()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	RootComponent = Mesh;

	TriggerZone->SetupAttachment(RootComponent); 
}

void ADoor::InteractSuccessful()
{
	// Do nothing if pending kill/destroy 
	if(GetLifeSpan() != 0)
		return; 
	
	Super::InteractSuccessful();

	// simply play audio and destroy the door for now, animation later 
	if(DoorOpenSound)
		UGameplayStatics::PlaySoundAtLocation(this, DoorOpenSound, InteractAudioPlayer->GetComponentLocation()); 

	// Destroy with delay so interact sound can fade out 
	SetLifeSpan(InteractFadeOutDuration);
	InteractAudioPlayer->FadeOut(InteractFadeOutDuration, 0);
}
