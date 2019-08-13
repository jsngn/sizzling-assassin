// Fill out your copyright notice in the Description page of Project Settings.


#include "Bacon.h"

// Sets default values
ABacon::ABacon()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform, USpringArmComponent::SocketName);  // Attach to socket at end of springarm
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
