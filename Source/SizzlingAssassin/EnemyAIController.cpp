// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "Bacon.h"
#include "Enemy.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "GreaseDrop.h"

AEnemyAIController::AEnemyAIController() {
	PrimaryActorTick.bCanEverTick = true;

	// Can change these to change turret's perception ability
	AISightRadius = 4000.0f;
	AILoseSightRadius = AISightRadius + 50.0f;
	AISightAge = 7.0f;
	AIFieldOfView = 360.0f;

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent")));

	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}

void AEnemyAIController::BeginPlay() {
	Super::BeginPlay();
}

void AEnemyAIController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	// Move to closest grease drop
	TArray<AActor*> ExistingGreaseDrops;
	TArray<float> DistanceArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGreaseDrop::StaticClass(), ExistingGreaseDrops);
	if (ExistingGreaseDrops.Num() > 0) {
		// Can only be distracted by grease drops nearby
		for (size_t i = 0; i < ExistingGreaseDrops.Num(); i++) {
			if (!LineOfSightTo(ExistingGreaseDrops[i])) {
				ExistingGreaseDrops.RemoveAt(i);
			}
		}

		for (size_t i = 0; i < ExistingGreaseDrops.Num(); i++) {
			DistanceArray.Emplace(GetPawn()->GetDistanceTo(ExistingGreaseDrops[i]));
		}

		DistanceArray.Sort();

		for (size_t i = 0; i < ExistingGreaseDrops.Num(); i++) {

			if ((GetPawn()->GetDistanceTo(ExistingGreaseDrops[i])) == DistanceArray[0]) {
				MoveToActor(ExistingGreaseDrops[i]);
			}
		}
	}
	else { // Only move to bacon if no grease drop
		TArray<AActor*> ExistingBacon;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABacon::StaticClass(), ExistingBacon);
		if (ExistingBacon.Num() > 0) {
			if (LineOfSightTo(ExistingBacon[0])) { // Only chase bacon if it can see bacon
				MoveToActor(ExistingBacon[0]);
			}
		}
	}
}

void AEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) {
	AEnemy* Enemy = Cast<AEnemy>(GetPawn());

	if (Enemy) {
		Enemy->Eat();
	}
}