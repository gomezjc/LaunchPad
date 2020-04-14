// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "LaunchPadCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Public/LP_LaunchPlatform.h"
#include "Public/LP_Panel.h"

//////////////////////////////////////////////////////////////////////////
// ALaunchPadCharacter

ALaunchPadCharacter::ALaunchPadCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ALaunchPadCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ALaunchPadCharacter::Interact);


	PlayerInputComponent->BindAxis("MoveForward", this, &ALaunchPadCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALaunchPadCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ALaunchPadCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ALaunchPadCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ALaunchPadCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ALaunchPadCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ALaunchPadCharacter::OnResetVR);
}

void ALaunchPadCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ALaunchPadCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ALaunchPadCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ALaunchPadCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ALaunchPadCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ALaunchPadCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ALaunchPadCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ALaunchPadCharacter::LaunchPlayer(FVector Direction)
{
	Super::LaunchCharacter(Direction * LaunchVelocity, true, true);
}

void ALaunchPadCharacter::Interact()
{
	if(bCanInteract)
	{
		TSubclassOf<ALP_LaunchPlatform> ClassToFind;
		ClassToFind = ALP_LaunchPlatform::StaticClass();
		TArray<AActor*> ElementsFound;
	
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, ElementsFound);

		for (int32 iCollected = 0; iCollected < ElementsFound.Num(); ++iCollected)
		{
			ALP_LaunchPlatform* Platform = Cast<ALP_LaunchPlatform>(ElementsFound[iCollected]);

			if (IsValid(Platform))
			{
				Platform->ChangePlatformState();
			}
		}

		TSubclassOf<ALP_Panel> ClassToFindPanel;
		ClassToFindPanel = ALP_Panel::StaticClass();
		TArray<AActor*> ElementsFoundPanel;

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFindPanel, ElementsFoundPanel);

		for (int32 iCollected = 0; iCollected < ElementsFoundPanel.Num(); ++iCollected)
		{
			ALP_Panel* ButtonPanel = Cast<ALP_Panel>(ElementsFoundPanel[iCollected]);

			if (IsValid(ButtonPanel))
			{
				ButtonPanel->ChangeButtonState();
			}
		}
	}
}