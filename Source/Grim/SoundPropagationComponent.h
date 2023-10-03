// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "SoundPropagationComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GRIM_API USoundPropagationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USoundPropagationComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	// The audio occlusion component that holds all audio comps in the level
	UPROPERTY()
	class UAudioOcclusionComponent* AudioOccComp; 

	void UpdateSoundPropagation(UAudioComponent* AudioComp);

	class FPathfinder* Pathfinder = nullptr;

	// Map containing the original audio component and the spawned, propagated audio component
	UPROPERTY()
	TMap<UAudioComponent*, UAudioComponent*> PropagatedSounds;

	bool DoLineTrace(FHitResult& HitResultOut, const FVector& StartLoc, const TArray<AActor*>& ActorsToIgnore) const;

	void RemovePropagatedSound(const UAudioComponent* AudioComp);

	void SpawnPropagatedSound(UAudioComponent* AudioComp, const FVector& SpawnLocation);

	UPROPERTY(EditAnywhere)
	USoundAttenuation* PropagatedSoundAttenuation = nullptr;  

};
