// Fill out your copyright notice in the Description page of Project Settings.


#include "HealingSpace.h"
#include "DrawDebugHelpers.h"
#include "Bacon.h"
#include "EnemySpawnVolume.h"

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
			// Healing bacon
			Bacon->bIsHealing = true;
			Bacon->bIsTimerSet = true;

			// Spawn enemies to chase bacon
			TArray<AActor*> OverlappingSpawnVolumes;
			TArray<AActor*>& OverlappingSpawnVolumesRef = OverlappingSpawnVolumes;
			GetOverlappingActors(OverlappingSpawnVolumesRef, AEnemySpawnVolume::StaticClass());
			FRotator BaconRot = Bacon->GetActorRotation();
			FRotator EnemySpawnRot = FRotator(BaconRot.Pitch, 180.0f + BaconRot.Yaw, BaconRot.Roll);

			for (size_t i = 0; i < OverlappingSpawnVolumes.Num(); i++) {
				if (AEnemySpawnVolume* SpawnVolume = Cast<AEnemySpawnVolume>(OverlappingSpawnVolumes[i])) {
					SpawnVolume->bShouldSpawn = true;
					SpawnVolume->SpawnRot = EnemySpawnRot;
					SpawnVolume->SpawnEnemies();
				}
			}
		}
	}
}

// Handles bacon leaving sunlight area
void AHealingSpace::OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor) {
	if (OtherActor && (OtherActor != this)) {
		if (ABacon * Bacon = Cast<ABacon>(OtherActor)) {
			// Stop healing bacon
			Bacon->bIsHealing = false;
			Bacon->bIsTimerSet = false;

			// No more spawning
			TArray<AActor*> OverlappingSpawnVolumes;
			TArray<AActor*>& OverlappingSpawnVolumesRef = OverlappingSpawnVolumes;
			GetOverlappingActors(OverlappingSpawnVolumesRef, AEnemySpawnVolume::StaticClass());
			for (size_t i = 0; i < OverlappingSpawnVolumes.Num(); i++) {
				if (AEnemySpawnVolume * SpawnVolume = Cast<AEnemySpawnVolume>(OverlappingSpawnVolumes[i])) {
					SpawnVolume->bShouldSpawn = false;
				}
			}
		}
	}
}