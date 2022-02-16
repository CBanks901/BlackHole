// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainBaseActor.generated.h"

UCLASS()
class BLACKHOLEREDO_API AMainBaseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMainBaseActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* MainSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Static_Mesh;

	UPROPERTY()
	class UBoxComponent* newComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite)
	bool NormalDirection;
};
