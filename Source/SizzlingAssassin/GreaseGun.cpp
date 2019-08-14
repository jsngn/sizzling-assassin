// Fill out your copyright notice in the Description page of Project Settings.


#include "GreaseGun.h"

// Sets default values
AGreaseGun::AGreaseGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGreaseGun::BeginPlay()
{
	Super::BeginPlay();
	
	// Initializes full ammo
	CurrentAmmo = FullAmmo;
}

// Called every frame
void AGreaseGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("Ammo: %i"), CurrentAmmo);
}

FText AGreaseGun::GetFullAmmoText() {
	// Convert to string first, then text
	FString FullAmmoString = FString::FromInt(FullAmmo);
	FText FullAmmoText = FText::FromString(FullAmmoString);
	return FullAmmoText;
}

FText AGreaseGun::GetCurrentAmmoText() {
	// Convert to string first, then text
	FString CurrentAmmoString = FString::FromInt(CurrentAmmo);
	FText CurrentAmmoText = FText::FromString(CurrentAmmoString);
	return CurrentAmmoText;
}

void AGreaseGun::DecreaseAmmo() {
	CurrentAmmo--;
}

void AGreaseGun::Reload_Implementation(int Amount) {
	CurrentAmmo += Amount;
}

int AGreaseGun::GetRefillAmount() {
	return FullAmmo - CurrentAmmo;
}

bool AGreaseGun::CanShoot() {
	return CurrentAmmo > 0;
}