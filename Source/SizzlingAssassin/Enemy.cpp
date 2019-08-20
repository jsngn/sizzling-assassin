// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/BoxComponent.h"
#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Bacon.h"
#include "Engine.h"
#include "DrawDebugHelpers.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CritHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CritHitBox"));
	CritHitBox->SetNotifyRigidBodyCollision(true);
	CritHitBox->BodyInstance.SetCollisionProfileName("BlockAllDynamic");
	CritHitBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	NormalHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("NormalHitBox"));
	NormalHitBox->SetNotifyRigidBodyCollision(true);
	NormalHitBox->BodyInstance.SetCollisionProfileName("BlockAllDynamic");
	NormalHitBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	MeleeSocket = CreateDefaultSubobject<USceneComponent>(TEXT("MeleeSocket"));
	MeleeSocket->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHealth = FullHealth;
	HealthPercentage = float(CurrentHealth) / float(FullHealth);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::Attacked(UPrimitiveComponent* HitComponent) {
	if (HitComponent == NormalHitBox) {
		UE_LOG(LogTemp, Warning, TEXT("Normal hit"));
		CurrentHealth -= NormalDamage;
		UE_LOG(LogTemp, Warning, TEXT("Health: %f"), CurrentHealth);
	}
	else if (HitComponent == CritHitBox) {
		UE_LOG(LogTemp, Warning, TEXT("Crit hit"));
		CurrentHealth -= CritDamage;
		UE_LOG(LogTemp, Warning, TEXT("Health: %f"), CurrentHealth);
	}

	if (CurrentHealth <= 0.0f) {
		Death();
	}
	else {
		HealthPercentage = float(CurrentHealth) / float(FullHealth);
	}
}

void AEnemy::Death_Implementation() {
	Destroy();
}

void AEnemy::Eat_Implementation() {
	FHitResult OutHit;
	FVector Start = MeleeSocket->GetComponentLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector End = ((ForwardVector * MeleeRange) + Start);
	FCollisionQueryParams CollisionParams;

	DrawDebugLine(GetWorld(), Start, End, FColor::Red, true);

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams)) {
		if (OutHit.bBlockingHit) {
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("You are hitting: %s"), *OutHit.GetActor()->GetName()));
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Purple, FString::Printf(TEXT("Impact Point: %s"), *OutHit.ImpactPoint.ToString()));
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("Normal Point: %s"), *OutHit.ImpactNormal.ToString()));

			if (ABacon* HitBacon = Cast<ABacon>(OutHit.GetActor())) {
				HitBacon->Eaten(); // Eats bacon if close enough to bacon
			}
		}
	}
}