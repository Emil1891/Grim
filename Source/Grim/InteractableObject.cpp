// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableObject.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AInteractableObject::AInteractableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false; // no need to tick as of now 

	TriggerZone = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TriggerZone"));
	TriggerZone->SetupAttachment(RootComponent); 

	// bind functions for when player enters/exits the trigger zone
	TriggerZone->OnComponentBeginOverlap.AddDynamic(this, &AInteractableObject::TriggerZoneEntered);
	TriggerZone->OnComponentEndOverlap.AddDynamic(this, &AInteractableObject::TriggerZoneExited);

	// Create the audio player that will play the interact looping sound 
	InteractAudioPlayer = CreateDefaultSubobject<UAudioComponent>(TEXT("InteractAudioPlayer")); 
	InteractAudioPlayer->SetupAttachment(RootComponent);
	InteractAudioPlayer->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AInteractableObject::BeginPlay()
{
	Super::BeginPlay();

	Player = UGameplayStatics::GetPlayerPawn(this, 0);

	// Enable input for this actor and bind function for when input button is pressed 
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0); 
	
	EnableInput(PlayerController);

	PlayerController->InputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed, this, &AInteractableObject::PlayerInteracted);

	InteractAudioPlayer->SetSound(InteractEnterSound); 
}

void AInteractableObject::PlayerInteracted()
{
	if(bPlayerIsInZone && bIsInteractable)
		InteractSuccessful(); 
}

void AInteractableObject::TriggerZoneEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// if not interactable or not colliding with player 
	if(!bIsInteractable || OtherActor != Player)
		return;

	bPlayerIsInZone = true;

	// Start playing interact zone sound by fading it in 
	InteractAudioPlayer->FadeIn(InteractFadeInDuration);  
}

void AInteractableObject::TriggerZoneExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int OtherBodyIndex)
{
	// if not interactable and player has exited the trigger zone after it was or not stopping collision with player 
	if((!bIsInteractable && !bPlayerIsInZone) || OtherActor != Player)
		return;

	bPlayerIsInZone = false;
	
	// Stop playing interact zone sound by fading it out 
	InteractAudioPlayer->FadeOut(InteractFadeOutDuration, 0); 
}

void AInteractableObject::InteractSuccessful()
{
	OnPlayerInteract();

	if(bCanOnlyInteractOnce)
		bIsInteractable = false;
}
