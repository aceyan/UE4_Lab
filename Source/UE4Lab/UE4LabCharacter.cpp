// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "UE4LabCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "Kismet/GameplayStatics.h"
#include "Engine.h"

//////////////////////////////////////////////////////////////////////////
// AUE4LabCharacter

AUE4LabCharacter::AUE4LabCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(Radius, HalfHeight);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->CrouchedHalfHeight = CrouchedHalfHeight;

	GetCharacterMovement()->MaxWalkSpeed = RunMoveSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchedMoveSpeed;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 200.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->bEnableCameraLag = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//Create a USkeletalMeshComponent for gun
	Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
	Gun->SetupAttachment(GetMesh(), TEXT("WeaponSocket"));


	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AUE4LabCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AUE4LabCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AUE4LabCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AUE4LabCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AUE4LabCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AUE4LabCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AUE4LabCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AUE4LabCharacter::OnResetVR);
}


void AUE4LabCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AUE4LabCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		//Jump();
}

void AUE4LabCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		//StopJumping();
}

void AUE4LabCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AUE4LabCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AUE4LabCharacter::MoveForward(float Value)
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

void AUE4LabCharacter::MoveRight(float Value)
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

void AUE4LabCharacter::Prone()
{
	 _IsProne = true;  
	 _IsGettingUPFromProne = false; 
	 GetCharacterMovement()->MaxWalkSpeed = ProneMoveSpeed;
	 //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("TouchLocation: %f"), GetCharacterMovement()->MaxWalkSpeed));
		//UE_LOG(LogTemp, Warning, TEXT("Prone"));

	 UGameplayStatics::GetPlayerCameraManager(this, 0)->ViewPitchMax = PronePitchMax;
	 UGameplayStatics::GetPlayerCameraManager(this, 0)->ViewPitchMin = PronePitchMin;
	 //

	 GetCapsuleComponent()->SetCapsuleRadius(ProneRadius);
	 GetCapsuleComponent()->SetCapsuleHalfHeight(ProneHalfHeight);
	 GetCapsuleComponent()->SetRelativeLocation(GetCapsuleComponent()->RelativeLocation - FVector(0,0, HalfHeight - ProneHalfHeight));
	 GetMesh()->SetRelativeLocation(FVector(0, 0, -ProneHalfHeight - 1));


	 GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("size: %s"), *GetMesh()->RelativeLocation.ToString()));
	
}

void AUE4LabCharacter::UnProne()
{
	_IsProne = false;
	_IsGettingUPFromProne = true;
	GetCharacterMovement()->MaxWalkSpeed = RunMoveSpeed;

	UGameplayStatics::GetPlayerCameraManager(this, 0)->ViewPitchMax = PitchMax;
	UGameplayStatics::GetPlayerCameraManager(this, 0)->ViewPitchMin = PitchMin;

	GetCapsuleComponent()->SetCapsuleRadius(Radius);
	GetCapsuleComponent()->SetCapsuleHalfHeight(HalfHeight);
	GetMesh()->SetRelativeLocation(FVector(0,0,-HalfHeight - 1));

}


void AUE4LabCharacter::FireStart()
{
	Fire();
}

void AUE4LabCharacter::FireStop()
{

}


void AUE4LabCharacter::Fire()
{
	if (_IsProne)
	{
		PlayAnimMontage(ProneFireMontage);
	}
	else
	{
		PlayAnimMontage(FireMontage);
	}
	
	UGameplayStatics::PlaySoundAtLocation(this, GunSound, GetActorLocation());
}