// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnQuestStarted();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnQuestComplete();
	
private:

	UPROPERTY(EditAnywhere)
	class AQuestDoor* QuestDoor;

	UPROPERTY() 
	AActor* CorrectPerson; // Juliet, is set (randomized) in begin play 

	UPROPERTY(EditAnywhere)
	AActor* JulietText; 

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
	UAudioComponent* AudioPlayerNarrator;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* TriggerZone;

	void PlayDelayedSound();

	UFUNCTION()
	void TriggerZoneEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							UPrimitiveComponent* OtherComponent, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY()
	APawn* Player;

	// The shorter version of the quest to repeat on command 
	UPROPERTY(EditAnywhere)
	USoundBase* QuestShortRepeat;

	void RepeatQuest();

	void DisableInteractionAndLowerVolume() const;

	// Sets Juliet by randomizing 
	void SetWhoIsJuliet();

	UPROPERTY()
	TArray<AActor*> People; 

	UPROPERTY(EditAnywhere)
	float TalkVolumeOnQuestComplete = 0.5f; 
};
