// Fill out your copyright notice in the Description page of Project Settings.


#include "GreaseDrop.h"
#include "TimerManager.h"

// Sets default values
AGreaseDrop::AGreaseDrop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGreaseDrop::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(DryTimer, this, &AGreaseDrop::DryUp, DryDelayTime);
}

// Called every frame
void AGreaseDrop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGreaseDrop::DryUp_Implementation() {
	Destroy();
}