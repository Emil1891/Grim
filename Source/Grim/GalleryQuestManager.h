// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableAudioPlayer.h"
#include "GalleryQuestManager.generated.h"

class AGalleryPerson;
/**
 * 
 */
UCLASS()
class GRIM_API AGalleryQuestManager : public AActor
{
	GENERATED_BODY()

public:

	AGalleryQuestManager(); 

	// Function called when player interacts with a person in the gallery level 
	void TalkedToPerson(const AGalleryPerson* Person);

	bool IsLevelComplete() const { return bLevelComplete; }

protected:
	virtual void BeginPlay() override;
	
private:

	UPROPERTY(EditAnywhere)
	class AQuestDoor* QuestDoor;

	UPROPERTY(EditAnywhere)
	AGalleryPerson* CorrectPerson; // Juliet

	bool bLevelComplete = false;

	void QuestCompleted();

	void TalkedToWrongPerson(); 
	
	UPROPERTY(EditAnywhere)
	TArray<USoundBase*> WrongSounds;

	// The sound meant to lure the player to go talk to Romeo 
	UPROPERTY(EditAnywhere)
	USoundBase* LureSound;

	UPROPERTY(EditAnywhere)
	USoundBase* QuestIntroSound;

	UPROPERTY(EditAnywhere)
	USoundBase* QuestSuccessSound; 

	UPROPERTY(EditAnywhere)
	UAudioComponent* AudioPlayer;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* TriggerZone;

	UFUNCTION()
	void TriggerZoneEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							UPrimitiveComponent* OtherComponent, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY()
	APawn* Player;

	// The shorter version of the quest to repeat on command 
	UPROPERTY(EditAnywhere)
	USoundBase* QuestShortRepeat;

	void RepeatQuest();

	void DisableInteractionInLevel() const; 
};
