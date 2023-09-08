// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BallRoomPuzzleManager.generated.h"

class AInteractableAudioPlayer; 

UCLASS()
class GRIM_API ABallRoomPuzzleManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABallRoomPuzzleManager();

	void PlayerInteractedWithInstrument(AInteractableAudioPlayer* Instrument); 

	bool IsPuzzleComplete() const { return bPuzzleComplete; } 
	
private:

	UPROPERTY(EditAnywhere)
	TArray<AInteractableAudioPlayer*> CorrectInstrumentOrder;

	TArray<AInteractableAudioPlayer*> PlayerInputOrder;

	// AQuestDoor QuestDoor; // Ska kallas vid quest complete

	// Rests puzzle upon failure 
	void ResetPuzzle();

	bool bPuzzleComplete = false;

	UPROPERTY(EditAnywhere)
	USoundBase* PuzzleFailedSound = nullptr; 
};
