// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundPropagationComponent.h"

#include "AudioOcclusionComponent.h"
#include "MapGrid.h"
#include "Pathfinder.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
USoundPropagationComponent::USoundPropagationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void USoundPropagationComponent::BeginPlay()
{
	Super::BeginPlay();

	AudioOccComp = GetOwner()->FindComponentByClass<UAudioOcclusionComponent>();
	if(!AudioOccComp)
		UE_LOG(LogTemp, Error, TEXT("No audio occlusion component added to player"))
	
	Pathfinder = new FPathfinder(Cast<AMapGrid>(UGameplayStatics::GetActorOfClass(this, AMapGrid::StaticClass())));
}

// Called every frame
void USoundPropagationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Update each audio component's sound propagation 
	for(const auto& AudioComp : AudioOccComp->AudioComponents)
	{
		UpdateSoundPropagation(AudioComp); 
	}
}

// TODO: TAKE LENGTH INTO ACCOUNT? IF PATH > LENGTH THEN DONT PROPAGATE SOUND?
// TODO: DONT PLAY SOUND AT ALL IF EXCEEDING AN EVEN GREATER LENGTH? 
void USoundPropagationComponent::UpdateSoundPropagation(UAudioComponent* AudioComp)
{
	auto StartTime = FDateTime::Now().GetMillisecond(); 
	TArray<FGridNode*> Path; 
	if(!Pathfinder->FindPath(AudioComp->GetComponentLocation(), GetOwner()->GetActorLocation(), Path))
	{
		// NO PATH FOUND, REMOVE EVENTUAL PROPAGATED SOUND (RESET EVERYTHING)
		return; 
	}

	// Path found, find where audio should be coming from (which is probably the last node in the path that has direct
	// line of sight to player)

	// Iterate through path 
	for(int i = 1; i < Path.Num() - 1; i++)
	{
		// TODO: FIND THE LAST NODE WITH LINE OF SIGHT TO PLAYER, THAT'S THE LOCATION TO PROPAGATE THE SOUND TO

		// Drawing the path 
		DrawDebugSphere(GetWorld(), Path[i]->GetWorldCoordinate(), 30, 10, FColor::Red);
	}

	auto EndTime = FDateTime::Now().GetMillisecond();
	if(EndTime - StartTime != 0)
		UE_LOG(LogTemp, Warning, TEXT("Update time: %i ms"), EndTime - StartTime)
}
