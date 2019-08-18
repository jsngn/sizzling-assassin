// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "HumanAIController.generated.h"

/**
 * 
 */
UCLASS()
class SIZZLINGASSASSIN_API AHumanAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
};
