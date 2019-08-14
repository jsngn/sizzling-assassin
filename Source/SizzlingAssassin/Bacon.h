// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Grease")
	bool bIsHealing;

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

	// PLEASE CALL PARENT FUNCTION AFTER BP IMPLEMENTATION
	UFUNCTION(BlueprintNativeEvent, Category = "Gun")
	void OnFire();

	// PLEASE CALL PARENT FUNCTION AFTER BP IMPLEMENTATION
	UFUNCTION(BlueprintNativeEvent, Category = "Gun")
	void Reload();

	// How far (maximum) can grease ammo travel
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	float ShootRange;

	UFUNCTION(BlueprintNativeEvent, Category = "Grease")
	void Heal();

	// Grease healed per tick while in sunlight
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grease")
	int HealRate;
};
