// Copyright Epic Games, Inc. All Rights Reserved.

#include "GrimCharacter.h"
#include "GrimProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"


//////////////////////////////////////////////////////////////////////////
// AGrimCharacter

AGrimCharacter::AGrimCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

}

void AGrimCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	SpawnLocation = GetActorLocation();
}

//////////////////////////////////////////////////////////////////////////// Input

void AGrimCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGrimCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGrimCharacter::Look);
		
		EnhancedInputComponent->BindAction(RepeatInstructionAction, ETriggerEvent::Triggered, this, &AGrimCharacter::RepeatInstructions);
	}
}

void AGrimCharacter::RepeatInstructions()
{
	if( CurrentAudioInstructions )
	{
		UGameplayStatics::PlaySound2D(this, CurrentAudioInstructions);
	}
}

void AGrimCharacter::SetCurrentAudioInstructions(USoundBase* NewAudioInstructions)
{
	CurrentAudioInstructions = NewAudioInstructions;
}

void AGrimCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AGrimCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		//AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AGrimCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool AGrimCharacter::GetHasRifle()
{
	return bHasRifle;
}

void AGrimCharacter::Respawn()
{
	if( !bIsDead )
	{
		SetActorEnableCollision(false);
		//SetActorLocation(SpawnLocation);
		FTimerHandle DelayHandle;
		FTimerDelegate DelayDelegate;
		DelayDelegate.BindLambda([this] {
			if( bIsDead == true )
			{
				UGameplayStatics::OpenLevel(this, FName(UGameplayStatics::GetCurrentLevelName(this)));
				SetActorEnableCollision(true);
			}
		});
	
		DisableInput(Cast<APlayerController>(GetController()));
		RespawnTrigger();
		GetWorldTimerManager().SetTimer(DelayHandle, DelayDelegate, 2.f, false);
		bIsDead = true;
	}
}

void AGrimCharacter::NotifyHit(
	UPrimitiveComponent* MyComp,
	AActor* Other,
	UPrimitiveComponent* OtherComp,
	bool bSelfMoved,
	FVector HitLocation,
	FVector HitNormal,
	FVector NormalImpulse,
	const FHitResult& Hit
) {
	if( bIsHit ) return;

	FTimerHandle DelayHandle;
	FTimerDelegate DelayDelegate;
	DelayDelegate.BindLambda([this] {
		bIsHit = false;
	});
	
	for( auto Tag : TagsToCheck )
	{
		if( Other->ActorHasTag(Tag) )
		{
			CollideTrigger(Tag);
			bIsHit = true;
			GetWorldTimerManager().SetTimer(DelayHandle, DelayDelegate, 1.f, false);
		}
	}
}
