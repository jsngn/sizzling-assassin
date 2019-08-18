// Fill out your copyright notice in the Description page of Project Settings.


#include "Human.h"
#include "Components/BoxComponent.h"
#include "Bacon.h"

// Sets default values
AHuman::AHuman()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DamageBoxSocket = CreateDefaultSubobject<USceneComponent>(TEXT("DamageBoxSocket"));
	DamageBoxSocket->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	DamageBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageBox"));
	DamageBox->SetNotifyRigidBodyCollision(true);
	DamageBox->BodyInstance.SetCollisionProfileName("BlockAllDynamic");
	DamageBox->AttachToComponent(DamageBoxSocket, FAttachmentTransformRules::KeepRelativeTransform);
	DamageBox->OnComponentHit.AddDynamic(this, &AHuman::OnBoxHit);
}

// Called when the game starts or when spawned
void AHuman::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHuman::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHuman::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AHuman::OnBoxHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	if (OtherActor && (OtherActor != this) && (OtherComp)) {
		if (ABacon * Bacon = Cast<ABacon>(OtherActor)) {
			Bacon->Perish();
		}
	}
}