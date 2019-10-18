// Fill out your copyright notice in the Description page of Project Settings.


#include "Human.h"
#include "Components/BoxComponent.h"
#include "Bacon.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Waypoint.h"
#include "HumanAIController.h"

// Sets default values
AHuman::AHuman()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AHuman::BeginPlay()
{
	Super::BeginPlay();
	
	// Gets TArray of waypoints for this human to move back and forth between
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaypoint::StaticClass(), AllWaypoints);
	for (size_t i = 0; i < AllWaypoints.Num(); i++) {
		if (AWaypoint * Waypoint = Cast<AWaypoint>(AllWaypoints[i])) {
			if (Waypoint->GetWaypointOrder() == CurrentWaypoint || Waypoint->GetWaypointOrder() == MaxWaypoint) {
				AccessibleWaypoints.Emplace(Waypoint);
			}
		}
	}

	Move();
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

void AHuman::Move_Implementation() {
	AHumanAIController* HumanAIController = Cast<AHumanAIController>(GetController());

	if (HumanAIController) {
		for (AActor* Waypoint : AccessibleWaypoints) {
			if (AWaypoint * PossibleTargetWaypoint = Cast<AWaypoint>(Waypoint)) {
				if (PossibleTargetWaypoint->GetWaypointOrder() == CurrentWaypoint) {
					HumanAIController->MoveToActor(PossibleTargetWaypoint);
					if (CurrentWaypoint == MinWaypoint) {
						CurrentWaypoint = MaxWaypoint;
					}
					else {
						CurrentWaypoint = MinWaypoint;
					}
					break;
				}
			}
		}
	}
}