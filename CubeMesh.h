// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainBaseActor.h"
#include "CubeMesh.generated.h"

/**
 * 
 */

UCLASS()
class BLACKHOLEREDO_API ACubeMesh : public AMainBaseActor
{
	GENERATED_BODY()
public:
	ACubeMesh();
	~ACubeMesh();

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void EmptyMethod();

	bool dOnceA;

protected:
	virtual void BeginPlay() override;

	virtual void Destroyed();
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void IncreaseItemCount();

	UPROPERTY()
	class ABlackHoleRedoGameModeBase* gamemode;
};

