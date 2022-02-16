// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlackHoleActor.generated.h"


class AMainBaseActor;
UCLASS()
class BLACKHOLEREDO_API ABlackHoleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlackHoleActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	class USceneComponent* CustomRoot;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Components)
	class USceneComponent* HolePosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	class USceneComponent* DirectionMirror;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = Components)
	class USplineComponent* customSpline;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<AMainBaseActor*> ActorArray;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool AllowCollisions;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float SplineTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float EndTime;

	UPROPERTY(BlueprintReadOnly, Category = Components)
	class UBoxComponent* BoxCollisionComp;

	UPROPERTY(BlueprintReadOnly)
	bool LastAddition;

	UPROPERTY()
	UStaticMesh* BaseCylinder;
		
	UPROPERTY()
	UStaticMesh* BaseCube;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float FlippedSplineTimeValue();

	void CustomDelayFunction();

	FTimerHandle slightdelay;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// For when other actors begin to overlap this zone
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int OtherBodyIndex, bool From_Sweep, const FHitResult& Sweep_Result);
};
