// Fill out your copyright notice in the Description page of Project Settings.

#include "Door.h"

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

	UE_LOG(LogTemp, Warning, TEXT("Open sesame"))

	// simply play audio and destroy the door for now, animation later 
	if(DoorOpenSound)
		UGameplayStatics::PlaySoundAtLocation(this, DoorOpenSound, GetActorLocation()); 
	
	Mesh->DestroyComponent();
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
}
