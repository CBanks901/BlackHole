// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeMesh.h"
#include "Kismet/GameplayStatics.h"
#include "BlackHoleRedoGameModeBase.h"
ACubeMesh::ACubeMesh()
{
}


ACubeMesh::~ACubeMesh()
{
}

void ACubeMesh::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	if (OtherActor != NULL)
	{
		if (AMainBaseActor* ref = Cast<AMainBaseActor>(OtherActor) )
		{
			if (dOnceA)
			{
				dOnceA = false;

				FVector newVec = FVector(Hit.Normal.X, Hit.Normal.Y, 1.0);
				ref->Static_Mesh->AddImpulse(newVec * 500, FName("None"), true);
				

				if (NormalDirection)
				{
					ref->NormalDirection = true;
					NormalDirection = false;
				}
				else
				{
					ref->NormalDirection = false;
					NormalDirection = true;
				}

				FTimerHandle TimerHandle;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACubeMesh::EmptyMethod, .2, false);
			}
		}
	}
}

void ACubeMesh::EmptyMethod()
{
	dOnceA = true;
}

void ACubeMesh::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(17);

	if (Static_Mesh->GetStaticMesh())
	{
		Static_Mesh->OnComponentHit.AddDynamic(this, &ACubeMesh::OnComponentHit);
	}

	dOnceA = true;

	// Initialize a reference to the main game mode
	gamemode = Cast<ABlackHoleRedoGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACubeMesh::IncreaseItemCount, .5f, false);
}

void ACubeMesh::Destroyed()
{
	if (IsValid(gamemode))
		gamemode->spawned_items -= 1;
	else
		UE_LOG(LogTemp, Warning, TEXT("You're deleing?"));
}

void ACubeMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACubeMesh::IncreaseItemCount()
{
	if (IsValid(gamemode))
	{
		gamemode->spawned_items += 1;
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Impossible to access the game mode?"));
}
