// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "Bacon.h"
#include "Enemy.h"
#include "TimerManager.h"

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

	if (GetPerceptionComponent()) {
		UE_LOG(LogTemp, Warning, TEXT("AI System Set"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("AI Error"));
	}
}

void AEnemyAIController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	// Don't move these
	TArray<AActor*> OutActors;
	TArray<AActor*>& OutActorsRef = OutActors;
	TArray<ABacon*> BaconDetected;

	// Gets all actors in FOV of turret and puts them in OutActors array
	GetPerceptionComponent()->GetCurrentlyPerceivedActors(*SightConfig->GetSenseImplementation(), OutActorsRef);
	
	if (OutActors.Num() > 0) {
		for (size_t i = 0; i < OutActors.Num(); i++) {
			ABacon* Bacon = Cast<ABacon>(OutActors[i]);
			if (Bacon) {
				MoveToActor(Bacon);
			}
		}
	}
}

void AEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) {
	//Attack();
	UE_LOG(LogTemp, Warning, TEXT("Move complete registered"));
}

void AEnemyAIController::Attack() {
	AEnemy* Enemy = Cast<AEnemy>(GetPawn());
	if (Enemy) {
		UE_LOG(LogTemp, Warning, TEXT("Can get pawn"));
		Enemy->Attack();
	}
}