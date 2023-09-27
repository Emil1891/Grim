// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AudioOcclusionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GRIM_API UAudioOcclusionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAudioOcclusionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Function to manually add an audio component to also occlude, only necessary for components no present in level
	 * on begin play. Checks if it is already added to prevent duplicates */
	void AddAudioComponentToOcclusion(UAudioComponent* AudioComponent); 

private: 

	// Array holding all audio components in the level 
	TArray<UAudioComponent*> AudioComponents;

	// Called in begin play to fill the array 
	void SetAudioComponents();
	
	// The class that are checked to see if they have an audio component, default = all actors 
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ActorClassToSearchFor = AActor::StaticClass();

	bool DoLineTrace(TArray<FHitResult>& HitResultsOut, const FVector& StartLocation, const FVector& EndLocation) const;

	// The camera's location will be the start location for all line traces as it is located in the player's "head" 
	UPROPERTY()
	class UCameraComponent* CameraComp = nullptr;

	void UpdateAudioComp(UAudioComponent* AudioComp);

	float GetOcclusionValue(const FHitResult& HitResult, const FHitResult& HitResultFromAudio) const;

	float GetDistanceToEdgeValue(const FHitResult& HitResult, const FHitResult& HitResultFromAudio) const;

	float GetThicknessValue(const FHitResult& HitResultFromPlayer, const FHitResult& HitResultFromAudio) const; 

	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectsToQuery;

	float MaxMeshDistanceToBlockAllAudio = 900.f;

	float MaxWidthDistanceToBlockAllAudio = 2000.f; 

	void ResetAudioComponentOnNoBlock(UAudioComponent* AudioComponent); 
};
