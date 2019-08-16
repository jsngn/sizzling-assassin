// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/BoxComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CritHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CritHitBox"));
	CritHitBox->SetSimulatePhysics(true);
	CritHitBox->SetNotifyRigidBodyCollision(true);
	CritHitBox->BodyInstance.SetCollisionProfileName("BlockAllDynamic");
	//CritHitBox->OnComponentHit.AddDynamic(this, &AEnemy::OnCritHit);
	CritHitBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	NormalHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("NormalHitBox"));
	NormalHitBox->SetSimulatePhysics(true);
	NormalHitBox->SetNotifyRigidBodyCollision(true);
	NormalHitBox->BodyInstance.SetCollisionProfileName("BlockAllDynamic");
	//NormalHitBox->OnComponentHit.AddDynamic(this, &AEnemy::OnNormalHit);
	NormalHitBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
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
}

void AEnemy::Death_Implementation() {
	Destroy();
}