// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Human.generated.h"

UCLASS()
class SIZZLINGASSASSIN_API AHuman : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHuman();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Updated throughout gameplay but initially set this to the first waypoint number the human should move to
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	int CurrentWaypoint;
	
	// Set this to the first waypoint number the human should move to
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	int MinWaypoint;

	// Set this to the last waypoint number the human should move to
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	int MaxWaypoint;

	// Array of all waypoints in map; not specific to this human
	UPROPERTY()
	TArray<AActor*> AllWaypoints;

	// Array of waypoints for this human; should have only 2 elements because moving back and forth
	UPROPERTY()
	TArray<AActor*> AccessibleWaypoints;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Move in a defined path of waypoints for this specific human
	UFUNCTION(BlueprintNativeEvent, Category = "Movement")
	void Move();
};
