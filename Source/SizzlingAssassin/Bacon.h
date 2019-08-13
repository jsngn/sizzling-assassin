// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Bacon.generated.h"

UCLASS()
class SIZZLINGASSASSIN_API ABacon : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABacon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Handles moving on x axis (forward & backward)
	UFUNCTION()
	void MoveX(float AxisValue);

	// Handles moving on y axis (left & right)
	UFUNCTION()
	void MoveY(float AxisValue);

private:
	//// Camera component for player controlled bacon
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bacon", meta = (AllowPrivateAccess = "true"))
	//UCameraComponent* Camera;

	//// Spring arm component to hold up camera
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bacon", meta = (AllowPrivateAccess = "true"))
	//USpringArmComponent* SpringArm;

	// Degrees of yaw rotated per tick
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bacon", meta = (AllowPrivateAccess = "true"))
	float YawRotSpeed;
};
