// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawnVolume.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"
#include "Enemy.h"
#include "Engine.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AEnemySpawnVolume::AEnemySpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Make the box where enemies will be spawned from
	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));
	RootComponent = SpawnBox;
}

// Called when the game starts or when spawned
void AEnemySpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
	bShouldSpawn = false;
	SpawnRot = FRotator::ZeroRotator;
}

// Called every frame
void AEnemySpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector AEnemySpawnVolume::GetSpawnLoc() {
	return UKismetMathLibrary::RandomPointInBoundingBox(SpawnBox->Bounds.Origin, SpawnBox->Bounds.BoxExtent);
}

void AEnemySpawnVolume::SpawnEnemies() {
	if (bShouldSpawn) { // So the function can stop looping when bacon steps out of sunlight
		if (EnemyToSpawn) {
			UWorld* const World = GetWorld();

			if (World) {
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = Instigator;
				FVector SpawnLoc = GetSpawnLoc();

				// Spawn multiple enemies at once
				for (size_t i = 0; i < SpawnRate; i++) {
					World->SpawnActor<AEnemy>(EnemyToSpawn, SpawnLoc, SpawnRot, SpawnParams);
				}
			
				// Keep spawning enemies with time delay between each spawn until bacon leaves sunlight
				GetWorldTimerManager().SetTimer(SpawnTimer, this, &AEnemySpawnVolume::SpawnEnemies, SpawnDelayTime, false);
			}
		}
	}
}