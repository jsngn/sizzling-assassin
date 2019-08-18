// Fill out your copyright notice in the Description page of Project Settings.


#include "HumanAIController.h"
#include "Human.h"

void AHumanAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) {
	// Calls function to move character to all (remaining) waypoints in array when a move action is done
	if (AHuman* Human = Cast<AHuman>(GetPawn())) {
		Human->Move();
	}
}