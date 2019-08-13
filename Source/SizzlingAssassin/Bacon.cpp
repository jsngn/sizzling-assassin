// Fill out your copyright notice in the Description page of Project Settings.


#include "Bacon.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine.h"

// Sets default values
ABacon::ABacon()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform, USpringArmComponent::SocketName); // Attach to socket at end of springarm*/
}

// Called when the game starts or when spawned
void ABacon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABacon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
		FVector CurrentMouseLoc, CurrentMouseDirection; // Location is a throwaway variable
		PlayerController->DeprojectMousePositionToWorld(CurrentMouseLoc, CurrentMouseDirection);
		FRotator CurrentMouseRot = CurrentMouseDirection.Rotation();

		FRotator CurrentBaconRot = GetActorRotation();

		FRotator NewBaconRot = FRotator(CurrentBaconRot.Pitch, CurrentMouseRot.Yaw, CurrentBaconRot.Roll);

		SetActorRotation(NewBaconRot);

		//float MaxDeltaYawPerTick = YawRotSpeed * DeltaTime;

		//if (MaxDeltaYawPerTick > FMath::Abs(NewBaconRot.Yaw - CurrentBaconRot.Yaw)) {
		//	SetActorRotation(NewBaconRot);
		//}
		//else {
		//	NewBaconRot.Yaw = CurrentBaconRot.Yaw + MaxDeltaYawPerTick * (NewBaconRot.Yaw - CurrentBaconRot.Yaw);
		//	SetActorRotation(NewBaconRot);
		//}
		FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
		FVector2D ViewportCenter = FVector2D(ViewportSize.X / 2, ViewportSize.Y / 2);
		PlayerController->SetMouseLocation(ViewportCenter.X, ViewportCenter.Y);
	}
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
