// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UE4LabCharacter.generated.h"

UCLASS(config=Game)
class AUE4LabCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	AUE4LabCharacter();


	virtual void Tick(float DeltaTime) override;
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveSpeed")
		float ProneMoveSpeed = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveSpeed")
		float RunMoveSpeed = 500;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveSpeed")
		float CrouchedMoveSpeed = 200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Category = "Pitch"))
		float PitchMin = -89.9;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Category = "Pitch"))
		float PitchMax = 89.9;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Category = "Pitch"))
		float PronePitchMin = 0.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Category = "Pitch"))
		float PronePitchMax = 35;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Category = "PlayerDimension"))
		float HalfHeight = 96;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Category = "PlayerDimension"))
		float Radius = 42;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Category = "PlayerDimension"))
		float CrouchedHalfHeight = 42;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Category = "PlayerDimension"))
		float ProneHalfHeight = 21;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Category = "PlayerDimension"))
		float ProneRadius = 21;

	UFUNCTION(BlueprintCallable, Category = "Prone")
		bool IsProne() const { return _IsProne; }
	UFUNCTION(BlueprintCallable, Category = "Prone")
		bool IsGettingUPFromProne() const { return _IsGettingUPFromProne; }
	UFUNCTION(BlueprintCallable, Category = "Prone")
		void Prone();
	UFUNCTION(BlueprintCallable, Category = "Prone")
		void UnProne();


	UFUNCTION(BlueprintCallable, Category = "Fire")
		void FireStart();
	UFUNCTION(BlueprintCallable, Category = "Fire")
		void FireStop();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fire")
		FVector FireRayHitPoint;//used for drawing aim corsshair
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fire")
		FHitResult AimingRayHitResult;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fire")
		bool IsFiring = false;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
		class USkeletalMeshComponent * Gun;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		class UAnimMontage *FireMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		class UAnimMontage *ProneFireMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		class USoundBase *GunSound;

protected:
		bool _IsProne = false;
		bool _IsGettingUPFromProne = true;

		FTimerHandle FireTimerHandle;
protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	void Fire();
};

