// Fill out your copyright notice in the Description page of Project Settings.


#include "MainBaseActor.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMainBaseActor::AMainBaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MainSceneRoot = CreateDefaultSubobject<USceneComponent>(FName("Root"));
	MainSceneRoot->SetWorldLocation(FVector(0, 0, 0));

	// The main static meshcomponent
	Static_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM"));

	// Make the scene component the root of this object
	RootComponent = MainSceneRoot;

	// disable tick by default
	SetActorTickEnabled(false);
	
	Static_Mesh->SetComponentTickEnabled(true);
	Static_Mesh->bTickInEditor = true;
	Static_Mesh->SetupAttachment(MainSceneRoot);
	Static_Mesh->SetWorldLocation(FVector(0, 0, 0) );
	Static_Mesh->SetSimulatePhysics(true);
	Static_Mesh->SetEnableGravity(true);
	Static_Mesh->SetNotifyRigidBodyCollision(true);
	Static_Mesh->BodyInstance.SetCollisionProfileName(FName("BlockAllDynamic"));

}

// Called when the game starts or when spawned
void AMainBaseActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMainBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

