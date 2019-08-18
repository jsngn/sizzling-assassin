// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnVolume.generated.h"

UCLASS()
class SIZZLINGASSASSIN_API AEnemySpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawnVolume();

	// Returns the box component specifying where to spawn
	FORCEINLINE class UBoxComponent* GetSpawnBox() const { return SpawnBox; }

	// Returns a random point in box to spawn enemies
	UFUNCTION(BlueprintPure, Category = "Spawning")
	FVector GetSpawnLoc();

	bool bShouldSpawn;

	UFUNCTION()
	void SpawnEnemies();

	// Changed throughout gameplay depending on bacon rotation
	FRotator SpawnRot;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Specifies the enemy to spawn; set this to enemy or ant
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class AEnemy> EnemyToSpawn;

	// Dummy timer handle for spawning
	FTimerHandle SpawnTimer;

	// Time interval between each enemy spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float SpawnDelayTime;

	// Specifies where enemies spawn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* SpawnBox;

	// Number of enemies spawned between each delay time interval
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int SpawnRate;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
