// Fill out your copyright notice in the Description page of Project Settings.


#include "Bacon.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine.h"
#include "DrawDebugHelpers.h"
#include "GreaseGun.h"
#include "TimerManager.h"
#include "GreaseDrop.h"
#include "Enemy.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

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

	// Create bullet socket where line traces start
	BulletSocket = CreateDefaultSubobject<USceneComponent>(TEXT("BulletSocket"));
	BulletSocket->AttachToComponent(ChildGun, FAttachmentTransformRules::KeepRelativeTransform);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform, USpringArmComponent::SocketName);
}

// Called when the game starts or when spawned
void ABacon::BeginPlay()
{
	Super::BeginPlay();
	
	// Initializes full grease
	CurrentGrease = FullGrease;
	GreasePercentage = float(CurrentGrease) / float(FullGrease);

	// False so bacon doesn't heal on spawn (outside healing space)
	bIsHealing = false;
	bIsTimerSet = false;
}

// Called every frame
void ABacon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
		// Calculate rotation of cursor in world
		FVector CurrentMouseLoc, CurrentMouseDirection;
		PlayerController->DeprojectMousePositionToWorld(CurrentMouseLoc, CurrentMouseDirection);
		FRotator CurrentMouseRot = CurrentMouseDirection.Rotation();

		// Get rotation of bacon in world
		FRotator CurrentBaconRot = GetActorRotation();

		// Calculate new rotation by substituting cursor's yaw into old bacon rotation
		FRotator NewBaconRot = FRotator(CurrentBaconRot.Pitch, CurrentMouseRot.Yaw, CurrentBaconRot.Roll);

		SetActorRotation(NewBaconRot);  // Turn the actual mesh (will turn camera too, but this is reset later)

		// Get reference to grease gun
		TArray<AActor*> ChildrenForAim;
		TArray<AActor*>& ChildrenForAimRef = ChildrenForAim;
		this->GetAllChildActors(ChildrenForAimRef, false);

		if (ChildrenForAim.Num() > 0) {

			if (AGreaseGun * Gun = Cast<AGreaseGun>(ChildrenForAim[0])) {
				// Reset camera rotation so that its pitch and yaw match those of the cursor so camera follows cursor
				FRotator CurrentCameraRot = Camera->GetComponentRotation();
				FRotator NewCameraRot = FRotator(CurrentMouseRot.Pitch, CurrentMouseRot.Yaw, CurrentCameraRot.Roll);
				Camera->SetWorldRotation(NewCameraRot);

				// Reset gun rotation so that the direction that gun points in is controlled by cursor/camera
				FRotator CurrentGunRot = Gun->GetActorRotation();
				FRotator NewGunRot = FRotator(NewCameraRot.Pitch, CurrentGunRot.Yaw, CurrentGunRot.Roll);
				Gun->SetActorRotation(NewGunRot);
			}
		}

		// Reset mouse to center of viewport
		// Calculated in tick in case viewport size changed mid-game
		FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
		FVector2D ViewportCenter = FVector2D(ViewportSize.X / 2, ViewportSize.Y / 2);
		PlayerController->SetMouseLocation(ViewportCenter.X, ViewportCenter.Y);
	}
	
	// Passes when enter sunlight area
	if (bIsHealing && bIsTimerSet && CurrentGrease < FullGrease) {
		Heal();
		bIsTimerSet = false; // Heal has a timer to repeat itself if applicable, so call only once
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
				FVector ForwardVector = Gun->GetActorForwardVector(); // Shoot forward relative to gun so gun's rotation controls where to shoot
				FVector End = ((ForwardVector * ShootRange) + Start);
				FCollisionQueryParams CollisionParams;

				DrawDebugLine(GetWorld(), Start, End, FColor::Green, true);

				if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams)) {
					if (OutHit.bBlockingHit) {
						GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("You are hitting: %s"), *OutHit.GetActor()->GetName()));
						GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Purple, FString::Printf(TEXT("Impact Point: %s"), *OutHit.ImpactPoint.ToString()));
						GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("Normal Point: %s"), *OutHit.ImpactNormal.ToString()));

						if (AEnemy* HitEnemy = Cast<AEnemy>(OutHit.GetActor())) {
							GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("You are hitting component: %s"), *OutHit.GetComponent()->GetName()));
							HitEnemy->Attacked(OutHit.GetComponent());
						}
						else {
							DropGrease(OutHit.ImpactPoint); // Drop grease bullet where line trace hit, if hits anything
						}
					}
				}
				else {
					DropGrease(End); // Drop grease at max range of line trace if hits nothing
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
				Perish();
			}
		}
	}
}

void ABacon::Heal_Implementation() {
	if (bIsHealing) { // Once bacon steps out of sunlight area, stop calling this function on a timer
		if ((CurrentGrease + HealRate) <= FullGrease) {
			CurrentGrease += HealRate;
			GetWorldTimerManager().SetTimer(HealTimer, this, &ABacon::Heal, HealDelayTime, false); // Slowly recharges grease rather than instantaneous
		}
		else {
			CurrentGrease = FullGrease;
		}
	}
}

void ABacon::DropGrease(FVector SpawnLoc) {
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;
	GetWorld()->SpawnActor<AGreaseDrop>(GreaseBullet, SpawnLoc, GetActorRotation(), SpawnParams);
}

void ABacon::Perish_Implementation() {
	Destroy();
}

void ABacon::Eaten_Implementation() {
	CurrentGrease -= PestDamage;

	if (CurrentGrease <= 0) {
		Perish();
	}
}