// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableObject.generated.h"

/**
 * Base class handling interactability, when player is in zone and presses particular button.
 * A function is called and an event is fired. Note that Tick is disabled as default. 
 */

UCLASS()
class GRIM_API AInteractableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// event fired when player interacts inside trigger/interactable zone 
	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayerInteract();

	/* function called when player interacts inside trigger/interactable zone, meant to be overridden by child classes
	* for more specific behaviour. Just be sure to still call parent function so event is fired */ 
	virtual void InteractSuccessful();
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* TriggerZone;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY()
	APawn* Player;

	void PlayerInteracted();

	// called when an actor enters the trigger zone
	UFUNCTION()
	void TriggerZoneEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							UPrimitiveComponent* OtherComponent, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// called when an actor exits the trigger zone
	UFUNCTION()
	void TriggerZoneExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							UPrimitiveComponent* OtherComponent, int OtherBodyIndex);

	bool bPlayerIsInZone = false;

	// Sound to play when the player enters the trigger zone and can interact 
	UPROPERTY(EditAnywhere)
	USoundBase* InteractEnterSound = nullptr;

	// Sound to play when player exits the trigger zone and can no longer interact 
	UPROPERTY(EditAnywhere)
	USoundBase* InteractExitSound = nullptr;

};
