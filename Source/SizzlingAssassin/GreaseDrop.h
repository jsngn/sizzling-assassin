// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GreaseDrop.generated.h"

UCLASS()
class SIZZLINGASSASSIN_API AGreaseDrop : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGreaseDrop();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Dummy timer handle
	FTimerHandle DryTimer;

	// How long after spawn should grease dry up (destroyed)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grease")
	float DryDelayTime;

	// PLEASE CALL PARENT FUNCTION AFTER BP IMPLEMENTATION
	UFUNCTION(BlueprintNativeEvent, Category = "Grease")
	void DryUp();
};
