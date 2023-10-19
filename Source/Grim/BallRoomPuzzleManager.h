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

	// Returns the current play time for the passed audio component or -1 if the audio component does not exist 
	float GetPlayTime(const UAudioComponent* AudioComp) const;
	
	void SetPlayTimes(UAudioComponent* IdleInstrument); 
	
private:

	UPROPERTY(EditAnywhere)
	TArray<AInteractableAudioPlayer*> CorrectInstrumentOrder;

	UPROPERTY()
	TArray<AInteractableAudioPlayer*> PlayerInputOrder;

	// Door to open upon quest complete 
	UPROPERTY(EditAnywhere)
	class AQuestDoor* QuestDoor; 

	// Rests puzzle upon failure 
	void ResetPuzzle();

	UFUNCTION()
	void OnPlayBackChanged(const USoundWave* PlayingSoundWave, float PlayBackPercent);

	bool bPuzzleComplete = false;

	UPROPERTY(EditAnywhere)
	USoundBase* PuzzleFailedSound = nullptr;

	// Map holding music instruments and their sound's current playtime
	UPROPERTY()
	TMap<UAudioComponent*, float> PlayTimes;

};
