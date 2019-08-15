// Fill out your copyright notice in the Description page of Project Settings.


#include "HealingSpace.h"
#include "DrawDebugHelpers.h"
#include "Bacon.h"

AHealingSpace::AHealingSpace() {
	OnActorBeginOverlap.AddDynamic(this, &AHealingSpace::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &AHealingSpace::OnOverlapEnd);
}

void AHealingSpace::BeginPlay() {
	Super::BeginPlay();
	
	DrawDebugBox(GetWorld(), GetActorLocation(), GetActorScale() * 100, FColor::Turquoise, true, -1, 0, 5);
}

// Handles bacon walking into sunlight area
void AHealingSpace::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor) {
	if (OtherActor && (OtherActor != this)) {
		if (ABacon* Bacon = Cast<ABacon>(OtherActor)) {
			Bacon->bIsHealing = true;
			Bacon->bIsTimerSet = true;
		}
	}
}

// Handles bacon leaving sunlight area
void AHealingSpace::OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor) {
	if (OtherActor && (OtherActor != this)) {
		if (ABacon * Bacon = Cast<ABacon>(OtherActor)) {
			Bacon->bIsHealing = false;
			Bacon->bIsTimerSet = false;
		}
	}
}