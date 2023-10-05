// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestDoor.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AQuestDoor::AQuestDoor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	RootComponent = Mesh;

	AudioPlayer = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Player"));
	AudioPlayer->SetupAttachment(RootComponent); 
}

// Called when the game starts or when spawned
void AQuestDoor::BeginPlay()
{
	Super::BeginPlay();
}

void AQuestDoor::QuestCompleted()
{
	if( DoorOpenSound )
	{
		UGameplayStatics::PlaySoundAtLocation(this, DoorOpenSound, GetActorLocation());
	}
	if(SignallingSound)
	{
		AudioPlayer->SetSound(SignallingSound);
		AudioPlayer->Play(); 
	}

	Mesh->DestroyComponent();
}
