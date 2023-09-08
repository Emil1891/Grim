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

	// Puzzle manager assigns itself if it is not already set 
	if(!PuzzleManager)
		PuzzleManager = Cast<ABallRoomPuzzleManager>(UGameplayStatics::GetActorOfClass(this, ABallRoomPuzzleManager::StaticClass()));

	if(!ActiveSound || !IdleSound)
		UE_LOG(LogTemp, Error, TEXT("No idle or active sound!"))
}

void ABallRoomInstrument::ChangeToIdleSound()
{
	AudioPlayer->SetSound(IdleSound); 
}

void ABallRoomInstrument::InteractSuccessful()
{
	// do nothing if puzzle is already complete or already playing the active sound 
	if(PuzzleManager->IsPuzzleComplete() || AudioPlayer->GetSound() == ActiveSound)
		return; 
	
	Super::InteractSuccessful();

	AudioPlayer->SetSound(ActiveSound);

	PuzzleManager->PlayerInteractedWithInstrument(this);
}
