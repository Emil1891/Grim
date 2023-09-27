// Fill out your copyright notice in the Description page of Project Settings.


#include "AudioOcclusionComponent.h"

#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UAudioOcclusionComponent::UAudioOcclusionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame. You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}

// Called when the game starts
void UAudioOcclusionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	SetAudioComponents();

	CameraComp = GetOwner()->FindComponentByClass<UCameraComponent>(); 
}

// Called every frame
void UAudioOcclusionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for(UAudioComponent* Audio : AudioComponents)
	{
		UpdateAudioComp(Audio); 
	}
}

void UAudioOcclusionComponent::AddAudioComponentToOcclusion(UAudioComponent* AudioComponent)
{
	// Add if it does not already exist in array 
	if(!AudioComponents.Contains(AudioComponent)) 
		AudioComponents.Add(AudioComponent); 
}

void UAudioOcclusionComponent::SetAudioComponents()
{
	// Find all actors of set class (default all actors)
	TArray<AActor*> AllFoundActors;
	UGameplayStatics::GetAllActorsOfClass(this, ActorClassToSearchFor, AllFoundActors);

	for(const auto Actor : AllFoundActors)
	{
		// If the actor has an audio component 
		if(auto AudioComp = Actor->FindComponentByClass<UAudioComponent>())
			if(AudioComp->GetOwner()->GetActorNameOrLabel().Equals("TestSound")) // TODO: REMOVE THIS, ONLY USED FOR TESTING 
				AudioComponents.Add(AudioComp); // Add it to the array
	}
}

bool UAudioOcclusionComponent::DoLineTrace(TArray<FHitResult>& HitResultsOut, const FVector& StartLocation, const FVector& EndLocation) const
{
	return UKismetSystemLibrary::LineTraceMultiForObjects(GetWorld(), StartLocation, EndLocation, ObjectsToQuery, false, TArray<AActor*>(), EDrawDebugTrace::ForOneFrame, HitResultsOut, true); 
}

void UAudioOcclusionComponent::UpdateAudioComp(UAudioComponent* AudioComp)
{
	TArray<FHitResult> HitResultsFromPlayer;
	// No blocking objects 
	if(!DoLineTrace(HitResultsFromPlayer, CameraComp->GetComponentLocation(), AudioComp->GetComponentLocation()))
	{
		// Reset values when not blocking 
		ResetAudioComponentOnNoBlock(AudioComp); 
		return;
	}

	// Used to calculate distances that rays travel within objects 
	TArray<FHitResult> HitResultsFromAudio;
	DoLineTrace(HitResultsFromAudio, AudioComp->GetComponentLocation(), CameraComp->GetComponentLocation());
	
	if(HitResultsFromAudio.Num() != HitResultsFromPlayer.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Ray trace hits not equals for player and audio!, Audio: %i - Player: %i"), HitResultsFromAudio.Num(), HitResultsFromPlayer.Num())
		return; 
	}

	// Reverse the hit results from the audio's perspective so they are in the same order as the player's 
	Algo::Reverse(HitResultsFromAudio); 

	float LowestOcclusionValue = 1; 
	for(int i = 0; i < HitResultsFromAudio.Num(); i++)
	{
		const float NewOccValue = GetOcclusionValue(HitResultsFromPlayer[i], HitResultsFromAudio[i]);
		LowestOcclusionValue = FMath::Min(LowestOcclusionValue, NewOccValue);
	}

	UE_LOG(LogTemp, Warning, TEXT("Volume: %f"), LowestOcclusionValue)
	AudioComp->SetVolumeMultiplier(LowestOcclusionValue); 
}

float UAudioOcclusionComponent::GetOcclusionValue(const FHitResult& HitResultFromPlayer, const FHitResult& HitResultFromAudio) const
{
	const float ThicknessValue = GetThicknessValue(HitResultFromPlayer, HitResultFromAudio);

	const float WidthValue = GetDistanceToEdgeValue(HitResultFromPlayer, HitResultFromAudio); 

	return ThicknessValue * WidthValue; 
}

float UAudioOcclusionComponent::GetDistanceToEdgeValue(const FHitResult& HitResult,
	const FHitResult& HitResultFromAudio) const
{
	FVector ClosestPointOnMeshToPlayer; // In world coordinates 
	HitResultFromAudio.GetComponent()->GetClosestPointOnCollision(CameraComp->GetComponentLocation(), ClosestPointOnMeshToPlayer);

	float DistanceFromImpactToClosestPoint = FVector::Dist(ClosestPointOnMeshToPlayer, HitResultFromAudio.ImpactPoint);

	DistanceFromImpactToClosestPoint /= MaxWidthDistanceToBlockAllAudio;

	DistanceFromImpactToClosestPoint = FMath::Clamp(DistanceFromImpactToClosestPoint, 0, 1);

	DistanceFromImpactToClosestPoint = 1 - DistanceFromImpactToClosestPoint; 
	
	UE_LOG(LogTemp, Warning, TEXT("Width value: %f"), DistanceFromImpactToClosestPoint)

	return DistanceFromImpactToClosestPoint;
}

float UAudioOcclusionComponent::GetThicknessValue(const FHitResult& HitResultFromPlayer, const FHitResult& HitResultFromAudio) const
{
	// Get how far the ray traveled through the blocking mesh 
	float RayTravelDistance = FVector::Dist(HitResultFromPlayer.ImpactPoint, HitResultFromAudio.ImpactPoint);
	
	UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), RayTravelDistance)

	// Divide by the max distance to travel before blocking all audio and clamp to get a value between 0 and 1 
	RayTravelDistance /= MaxMeshDistanceToBlockAllAudio; 
	RayTravelDistance = FMath::Clamp(RayTravelDistance, 0, 1); 
	// "Reverse" so full block means 0 audio volume and vice versa 
	RayTravelDistance = 1 - RayTravelDistance; 

	UE_LOG(LogTemp, Warning, TEXT("RayTravelDistance: %f"), FMath::Clamp(RayTravelDistance, 0, 1)) 

	return RayTravelDistance; 
}

void UAudioOcclusionComponent::ResetAudioComponentOnNoBlock(UAudioComponent* AudioComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("Volume: 1"))
	AudioComponent->SetVolumeMultiplier(1); 
}
