// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "BasePawn.generated.h"

UCLASS()
class BLACKHOLEREDO_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	USceneComponent* OurVisibleComponent;

	UPROPERTY(EditAnywhere)
	UCameraComponent* OurCamera;

	FVector CharacterSpeed;
	FVector MoveRightVec;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);

	void YawInput(float input);
	void PitchInput(float input);

	void EndGame();
	float Yaw, Pitch;

};
