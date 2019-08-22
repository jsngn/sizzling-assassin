// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Bacon.generated.h"

UCLASS()
class SIZZLINGASSASSIN_API ABacon : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABacon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Handles moving on x axis (forward & backward)
	UFUNCTION()
	void MoveX(float AxisValue);

	// Handles moving on y axis (left & right)
	UFUNCTION()
	void MoveY(float AxisValue);

	// Full health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grease")
	int FullGrease;
	
	// For calculation for HUD
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Grease")
	int CurrentGrease;

	// For HUD
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Grease")
	float GreasePercentage;

	// For HUD
	UFUNCTION(BlueprintPure, Category = "Grease")
	FText GetFullGreaseText();

	// For HUD
	UFUNCTION(BlueprintPure, Category = "Grease")
	FText GetCurrentGreaseText();

	// Indicates whether bacon should still be healed
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Grease")
	bool bIsHealing;

	// Indicates whether timer for healing has been set once when bacon first steps into sunlight
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Grease")
	bool bIsTimerSet;

	// Takes pest damage
	UFUNCTION(BlueprintNativeEvent, Category = "Damage")
	void Eaten();

	// Do effects for death
	UFUNCTION(BlueprintNativeEvent, Category = "Damage")
	void Perish();

	// For HUD
	UFUNCTION(BlueprintPure, Category = "Ammo")
	FText GetFullAmmoText();

	// For HUD
	UFUNCTION(BlueprintPure, Category = "Ammo")
	FText GetCurrentAmmoText();

protected:
	// Dummy gun socket to attach grease gun child actor
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gun")
	USceneComponent* GunSocket;

	// Grease gun component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gun")
	UChildActorComponent* ChildGun;

	// Dummy bullet socket where grease bullets spawn/line trace starts
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gun")
	USceneComponent* BulletSocket;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpringArm")
	class USpringArmComponent* SpringArm;

	// PLEASE CALL PARENT FUNCTION AFTER BP IMPLEMENTATION
	UFUNCTION(BlueprintNativeEvent, Category = "Gun")
	void OnFire();

	// PLEASE CALL PARENT FUNCTION AFTER BP IMPLEMENTATION
	UFUNCTION(BlueprintNativeEvent, Category = "Gun")
	void Reload();

	// How far (maximum) can grease ammo travel
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	float ShootRange;

	// Handles healing in sunlight; PLEASE CALL PARENT FUNCTION AFTER BP IMPLEMENTATION
	UFUNCTION(BlueprintNativeEvent, Category = "Grease")
	void Heal();

	// Dummy timer handle
	FTimerHandle HealTimer;

	// Time interval between each heal action
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grease")
	float HealDelayTime;

	// Grease healed per heal action while in sunlight
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grease")
	int HealRate;

	// Class of the grease drop that spawns when did not shoot at enemy
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gun")
	TSubclassOf<class AGreaseDrop> GreaseBullet;

	// Spawns grease bullet on ground if line trace misses enemy
	UFUNCTION()
	void DropGrease(FVector SpawnLoc);

	// How much damage can a pest do to bacon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float PestDamage;

	// True when in game and unpaused
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aiming")
	bool bIsAiming;
};
