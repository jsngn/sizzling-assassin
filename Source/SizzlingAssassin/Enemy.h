// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class SIZZLINGASSASSIN_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Attacked(UPrimitiveComponent* HitComponent);

	// Max health of enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float FullHealth;

	// Update this throughout gameplay
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
	float CurrentHealth;

	// For HUD
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
	float HealthPercentage;

	UFUNCTION(BlueprintNativeEvent, Category = "Attack")
	void Eat();

protected:
	// Collision box for handling critical hits
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HitBox")
	class UBoxComponent* CritHitBox;

	// Collision box for handling normal hits
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HitBox")
	class UBoxComponent* NormalHitBox;

	// Dummy socket where melee line trace starts, should be in front of capsule
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Damage")
	USceneComponent* MeleeSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float CritDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float NormalDamage;

	// Do effects for death
	UFUNCTION(BlueprintNativeEvent, Category = "Damage")
	void Death();

	// Small value because melee (this is range of line trace)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float MeleeRange;
};