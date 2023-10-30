// Fill out your copyright notice in the Description page of Project Settings.

#include "GalleryQuestManager.h"

#include "GalleryPerson.h"
#include "QuestDoor.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

AGalleryQuestManager::AGalleryQuestManager()
{
	// Create the trigger zone 
	TriggerZone = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Trigger zone"));
	RootComponent = TriggerZone; 
	TriggerZone->SetCollisionResponseToAllChannels(ECR_Overlap); // Set to overlap
	TriggerZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // And to only query 

	// Bind function on overlap 
	TriggerZone->OnComponentBeginOverlap.AddDynamic(this, &AGalleryQuestManager::TriggerZoneEntered);

	// Create the audio player 
	//AudioPlayer = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Player"));

	//AudioPlayer->SetupAttachment(RootComponent);
}

void AGalleryQuestManager::BeginPlay()
{
	Super::BeginPlay();

	AudioPlayer = FindComponentByClass<UAudioComponent>(); 
	
	// Set and play the lure sound
	FTimerHandle TimerHandleIntro;
	GetWorldTimerManager().SetTimer(TimerHandleIntro, this, &AGalleryQuestManager::PlayDelayedSound, 19);

	Player = UGameplayStatics::GetPlayerPawn(this, 0);

	Player->InputComponent->BindAction("RepeatQuest", IE_Pressed, this, &AGalleryQuestManager::RepeatQuest);

	SetWhoIsJuliet();

	AssignTalkingSounds(); 
}

void AGalleryQuestManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorldTimerManager().ClearAllTimersForObject(this);
}

void AGalleryQuestManager::SetWhoIsJuliet()
{
	TArray<AActor*> PeopleActors; 
	UGameplayStatics::GetAllActorsOfClass(this, AGalleryPerson::StaticClass(), PeopleActors);

	for(const auto Person : PeopleActors)
	{
		if(auto PersonGallery = Cast<AGalleryPerson>(Person))
			People.Add(PersonGallery); 
	}

	CorrectPerson = People[FMath::RandRange(0, People.Num() - 1)];

	// Update the text's location to easily see who Juliet is (for us) 
	FVector NewTextLoc = CorrectPerson->GetActorLocation();
	NewTextLoc.Z = JulietText->GetActorLocation().Z;
	NewTextLoc.Y -= 100.f; // Offset so text is centred above the person 
	JulietText->SetActorLocation(NewTextLoc); 
}

void AGalleryQuestManager::AssignTalkingSounds()
{
	int WrongTalkCounter = 0; 
	for(const auto Person : People)
	{
		if(Person == CorrectPerson)
			Person->SetSound(JulietTalkingSound);
		else
			Person->SetSound(WrongTalkingSounds[WrongTalkCounter++]); 
	}
}


void AGalleryQuestManager::TalkedToPerson(const AGalleryPerson* Person)
{
	// Do nothing if level is already complete 
	if(bLevelComplete)
		return; 
	
	if(Person == CorrectPerson)
		QuestCompleted();
	else
		TalkedToWrongPerson(); 
}

void AGalleryQuestManager::QuestCompleted()
{
	OnQuestComplete();
	bLevelComplete = true;
	DisableInteractionAndLowerVolume();
	
	QuestDoor->QuestCompleted(); // Open door
	CorrectPerson->Destroy();
	AudioPlayer->SetSound(QuestSuccessSound);
	AudioPlayer->Play();
	
}

void AGalleryQuestManager::TalkedToWrongPerson()
{
	// Get a random sound from the potential sounds and play it 
	const auto SoundToPlay = WrongSounds[FMath::RandRange(0, WrongSounds.Num() - 1)]; 
	//AudioPlayer->SetSound(SoundToPlay);
	UGameplayStatics::SpawnSound2D(this, SoundToPlay);
	TalkedToWrongPersonEvent();
}

void AGalleryQuestManager::TriggerZoneEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	// DO nothing if the collision was not with player 
	if(OtherActor != Player)
		return; 
	
	AudioPlayer->SetSound(QuestIntroSound);
	FTimerHandle TimerHandleRepeater;
	GetWorldTimerManager().SetTimer(TimerHandleRepeater, this, &AGalleryQuestManager::SetQuestIntroFinished, 28);
	AudioPlayer->Play();
	OnQuestStarted();
	bQuestStarted = true; 
	// Destroy the trigger zone, as it is not needed anymore, quest is repeated when player presses the designated button
	TriggerZone->DestroyComponent();
}

void AGalleryQuestManager::RepeatQuest()
{
	// Do not play if quest is complete 
	if(bLevelComplete || bIntroComplete == false)
		return;

	OnReminderStarted();
	AudioPlayer->SetSound(QuestShortRepeat);
	AudioPlayer->Play(); 
}

void AGalleryQuestManager::DisableInteractionAndLowerVolume() const
{
	// Set interactable to false 
	for(const auto Person : People)
		if(const auto GalleryPerson = Cast<AGalleryPerson>(Person))
		{
			GalleryPerson->SetIsInteractable(false);
			// and lower volume for everyone 
			GalleryPerson->GetAudioComponent()->SetVolumeMultiplier(TalkVolumeOnQuestComplete); 
		}
}

void AGalleryQuestManager::PlayDelayedSound()
{
	if(bQuestStarted)
		return;
	
	AudioPlayer->SetSound(LureSound);
	AudioPlayer->Play();

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AGalleryQuestManager::PlayDelayedSound, LureDelayTime); 
}

void AGalleryQuestManager::SetQuestIntroFinished()
{
	bIntroComplete = true;
}
