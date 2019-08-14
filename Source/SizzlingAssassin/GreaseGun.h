// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GreaseGun.generated.h"

UCLASS()
class SIZZLINGASSASSIN_API AGreaseGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGreaseGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int FullAmmo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ammo")
	int CurrentAmmo;

	// For HUD
	UFUNCTION(BlueprintPure, Category = "Ammo")
	FText GetFullAmmoText();

	// For HUD
	UFUNCTION(BlueprintPure, Category = "Ammo")
	FText GetCurrentAmmoText();

	// Called once per fire
	UFUNCTION()
	void DecreaseAmmo();

	// PLEASE CALL PARENT FUNCTION AFTER BP IMPLEMENTATION
	UFUNCTION(BlueprintNativeEvent, Category = "Ammo")
	void Reload(int Amount);

	// For calculating reload amount; will always attempt to fill up ammo to max
	UFUNCTION()
	int GetRefillAmount();
	
	// Checks whether player has any ammo left
	UFUNCTION()
	bool CanShoot();
};
