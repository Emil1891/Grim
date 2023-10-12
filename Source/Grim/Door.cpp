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
	Super::InteractSuccessful();

	// simply play audio and destroy the door for now, animation later 
	if(DoorOpenSound)
		UGameplayStatics::PlaySoundAtLocation(this, DoorOpenSound, GetActorLocation()); 

	// Destroy with delay so interact sound can fade out 
	SetLifeSpan(InteractFadeOutDuration);
	InteractAudioPlayer->FadeOut(InteractFadeOutDuration, 0); 
}
