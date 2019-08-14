// Fill out your copyright notice in the Description page of Project Settings.


#include "Bacon.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine.h"
#include "DrawDebugHelpers.h"
#include "GreaseGun.h"

// Sets default values
ABacon::ABacon()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create gun socket then attach gun to it
	GunSocket = CreateDefaultSubobject<USceneComponent>(TEXT("GunSocket"));
	GunSocket->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ChildGun = CreateDefaultSubobject<UChildActorComponent>(TEXT("ChildGun"));
	ChildGun->AttachToComponent(GunSocket, FAttachmentTransformRules::KeepRelativeTransform);

	// Create bullet socket
	BulletSocket = CreateDefaultSubobject<USceneComponent>(TEXT("BulletSocket"));
	BulletSocket->AttachToComponent(ChildGun, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void ABacon::BeginPlay()
{
	Super::BeginPlay();
	
	// Initializes full grease
	CurrentGrease = FullGrease;
	GreasePercentage = float(CurrentGrease) / float(FullGrease);

	bIsHealing = false;
}

// Called every frame
void ABacon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
		// Calculate rotation of cursor in world
		FVector CurrentMouseLoc, CurrentMouseDirection; // Location is a throwaway variable
		PlayerController->DeprojectMousePositionToWorld(CurrentMouseLoc, CurrentMouseDirection);
		FRotator CurrentMouseRot = CurrentMouseDirection.Rotation();

		// Get rotation of bacon in world
		FRotator CurrentBaconRot = GetActorRotation();

		// Calculate new rotation by substituting cursor's yaw into old bacon rotation
		FRotator NewBaconRot = FRotator(CurrentBaconRot.Pitch, CurrentMouseRot.Yaw, CurrentBaconRot.Roll);

		SetActorRotation(NewBaconRot);
		
		// Reset mouse to center of viewport
		// Calculated in tick in case viewport size changed mid-game
		FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
		FVector2D ViewportCenter = FVector2D(ViewportSize.X / 2, ViewportSize.Y / 2);
		PlayerController->SetMouseLocation(ViewportCenter.X, ViewportCenter.Y);
	}

	if (bIsHealing && CurrentGrease < FullGrease) {
		Heal();
	}
	UE_LOG(LogTemp, Warning, TEXT("Health: %i"), CurrentGrease);
}

// Called to bind functionality to input
void ABacon::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Handles x & y movement
	PlayerInputComponent->BindAxis("MoveX", this, &ABacon::MoveX);
	PlayerInputComponent->BindAxis("MoveY", this, &ABacon::MoveY);
	
	// Handles jumping
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Handles interactions with grease gun
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABacon::OnFire);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ABacon::Reload);
}

void ABacon::MoveX(float AxisValue) {
	if (AxisValue != 0.0f) {
		AddMovementInput(GetActorForwardVector(), AxisValue);
	}
}

void ABacon::MoveY(float AxisValue) {
	if (AxisValue != 0.0f) {
		AddMovementInput(GetActorRightVector(), AxisValue);
	}
}

FText ABacon::GetFullGreaseText() {
	// Convert to string first, then text
	FString FullGreaseString = FString::FromInt(FullGrease);
	FText FullGreaseText = FText::FromString(FullGreaseString);
	return FullGreaseText;
}

FText ABacon::GetCurrentGreaseText() {
	// Convert to string first, then text
	FString CurrentGreaseString = FString::FromInt(CurrentGrease);
	FText CurrentGreaseText = FText::FromString(CurrentGreaseString);
	return CurrentGreaseText;
}

void ABacon::OnFire_Implementation() {
	// Get reference to grease gun
	TArray<AActor*> ChildrenForFire;
	TArray<AActor*>& ChildrenForFireRef = ChildrenForFire;
	this->GetAllChildActors(ChildrenForFireRef, false);

	if (ChildrenForFire.Num() > 0) {

		if (AGreaseGun* Gun = Cast<AGreaseGun>(ChildrenForFire[0])) {
			
			// Fire if grease gun has any ammo left, otherwise ignore input
			if (Gun->CanShoot()) {
				Gun->DecreaseAmmo();
			
				FHitResult OutHit;
				FVector Start = BulletSocket->GetComponentLocation();
				FVector ForwardVector = GetActorForwardVector();
				FVector End = ((ForwardVector * ShootRange) + Start); // Shoot in direction bacon faces
				FCollisionQueryParams CollisionParams;

				DrawDebugLine(GetWorld(), Start, End, FColor::Green, true);

				if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
				{
					if (OutHit.bBlockingHit)
					{
						GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("You are hitting: %s"), *OutHit.GetActor()->GetName()));
						GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Purple, FString::Printf(TEXT("Impact Point: %s"), *OutHit.ImpactPoint.ToString()));
						GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("Normal Point: %s"), *OutHit.ImpactNormal.ToString()));
					}
				}
			}
		}
	}

}

void ABacon::Reload_Implementation() {
	// Get reference to grease gun
	TArray<AActor*> ChildrenForReload;
	TArray<AActor*>& ChildrenForReloadRef = ChildrenForReload;
	this->GetAllChildActors(ChildrenForReloadRef, false);

	if (ChildrenForReload.Num() > 0) {

		if (AGreaseGun * Gun = Cast<AGreaseGun>(ChildrenForReload[0])) {
			// Attempt to refill gun to max ammo, if failed then bacon dies
			int Refill = Gun->GetRefillAmount();

			if (Refill <= CurrentGrease) {
				CurrentGrease -= Refill;
				Gun->Reload(Refill);
			}
			else {
				Destroy();
			}
		}
	}
}

void ABacon::Heal_Implementation() {
	if ((CurrentGrease + HealRate) <= FullGrease) {
		CurrentGrease += HealRate;
	}
	else {
		CurrentGrease = FullGrease;
	}
}