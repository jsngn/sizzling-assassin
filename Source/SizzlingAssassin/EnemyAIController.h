// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class SIZZLINGASSASSIN_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyAI")
	float AISightRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyAI")
	float AILoseSightRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyAI")
	float AISightAge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyAI")
	float AIFieldOfView;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemyAI")
	class UAISenseConfig_Sight* SightConfig;

	// Time between each attack action
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float AttackDelayTime;

	// Dummy handle
	FTimerHandle AttackTimer;

	UFUNCTION()
	void Attack();
};
