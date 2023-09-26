// Fill out your copyright notice in the Description page of Project Settings.


#include "GalleryPerson.h"

#include "GalleryQuestManager.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

void AGalleryPerson::SetSound(USoundBase* TalkSound) const
{
	AudioPlayer->SetSound(TalkSound); 
}

void AGalleryPerson::BeginPlay()
{
	Super::BeginPlay();

	if(!QuestManager)
		QuestManager = Cast<AGalleryQuestManager>(UGameplayStatics::GetActorOfClass(this, AGalleryQuestManager::StaticClass()));

	// GetDuration() returns an arbitrary large number if the sound is set to looping so need to read duration directly 
	const float RandomStartTime = FMath::RandRange(0.f, AudioPlayer->GetSound()->Duration);
	AudioPlayer->Play(RandomStartTime); 
}

void AGalleryPerson::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	QuestManager = nullptr; 
}

void AGalleryPerson::InteractSuccessful()
{
	Super::InteractSuccessful();

	QuestManager->TalkedToPerson(this); 
}
