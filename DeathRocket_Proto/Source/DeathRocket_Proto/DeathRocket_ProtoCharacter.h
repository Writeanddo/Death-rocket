// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DeathRocket_ProtoCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHealthEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAmmoEvent);

UCLASS(config=Game)
class ADeathRocket_ProtoCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* RocketLauncher;

public:
	ADeathRocket_ProtoCharacter();
	~ADeathRocket_ProtoCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ARocket> rocketClass;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Jump() override;

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	// FIRE AND RELOAD
	bool firing = false;
	bool reloading = false;
	bool stopMovement = false;

	UPROPERTY(EditAnywhere, Category = Weapon)
	float fireRate = 1.f;
	UPROPERTY(EditAnywhere, Category = Weapon)
	float reloadTime = 5.f;

	class Timer* fireTimer;
	class Timer* reloadTimer;
	// for the UI
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float fireProgress = 0.f;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float reloadProgress = 0.f;

	void Fire();
	void EndFire();
	void Reload();
	void EndReload();
	void UpdateTimersProgress();

	// CAMERA
	int   shoulder = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float cameraYOffset = 30.f;
	// Aim Down Sight (when aiming)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float ads = 50.f;
	// Run fov (when running)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float runFov = 110.f;
	// BASIC Field of view (when not aiming)
	float fov;
	float curFov;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float healthRatio = 1.f;

	// SPRINT
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool dashActivate = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float walkingSpeed = 600.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float runningSpeed = 800.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float dashingSpeed = 8000.f;

	bool  sprinting = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float dashMaxTime = 0.05f;
	float curSprintTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float dashRecoveryTime = 1.f;
	bool dashRecovering = false;
	class Timer* dashRecoveryTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float maxStamina = 100.f;
	float curStamina;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float staminaRatio = 1.f;
	bool staminaRecup = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float consumptionSeconds = 40.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float recuperationSeconds = 30.f;

	// AMMUNITION
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	int   ammoMax = 3;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int   curAmmo;

	UPROPERTY(BlueprintAssignable, Category = Event)
	FAmmoEvent OnAmmoUpdate;

	// METHODS
	void changeCamSide();
	void Aim();
	void StopAiming();

	void TakeDamage();

	UFUNCTION()
	void OnDeath();

	void Sprint();
	void StopSprint();
	void RecoverDash();


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UHealthComponent* healthComp;

	virtual void Tick(float DeltaTime) override;

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
