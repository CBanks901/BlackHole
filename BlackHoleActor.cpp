// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackHoleActor.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "CubeMesh.h"
#include "BlackHoleRedoGameModeBase.h"
#include "MainBaseActor.h"


// Sets default values
ABlackHoleActor::ABlackHoleActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(false);
	CustomRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	CustomRoot->SetWorldLocation(FVector(0, 0, 0));
	RootComponent = this->CustomRoot;

	//
	HolePosition = CreateDefaultSubobject<USceneComponent>(FName("Helper"));
	HolePosition->SetupAttachment(CustomRoot);
	HolePosition->SetRelativeLocation(FVector(0, 0, 100));

	// Set up the mirror scene component, initialize its world location and attach it to the root
	DirectionMirror = CreateDefaultSubobject<USceneComponent>(TEXT("MirrorLocation"));
	//DirectionMirror->SetWorldLocation(FVector(0, 0, 0));
	DirectionMirror->SetupAttachment(CustomRoot);
	DirectionMirror->SetRelativeLocation(FVector(0, 0, 100));

	// Create the box component, set its location above a certain height, and finally attach it to the root
	BoxCollisionComp = CreateDefaultSubobject<UBoxComponent>(FName("Collision"));
	BoxCollisionComp->SetWorldLocation(FVector(0, 0, 110) );
	BoxCollisionComp->SetupAttachment(CustomRoot);
	
	// Stretch the box out here
	BoxCollisionComp->SetBoxExtent(FVector(700, 700, 100));

	AllowCollisions = true;
	LastAddition = false;

	customSpline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	customSpline->SetupAttachment(CustomRoot);
	customSpline->SetRelativeLocation(FVector(-370, 0, 260));
	
	customSpline->SetClosedLoop(true);

	// Clear the default spline and add new based locally to the spline
	customSpline->ClearSplinePoints();
	customSpline->AddSplinePoint(FVector(-89.979538, 1.889979, 80), ESplineCoordinateSpace::Local);
	customSpline->AddSplinePoint(FVector(61.457996, 233.428894, 0), ESplineCoordinateSpace::Local);
	customSpline->AddSplinePoint(FVector(549.934326, -7.602399, 100), ESplineCoordinateSpace::Local);
	customSpline->AddSplinePoint(FVector(49.977882, -489.968689, 0.0), ESplineCoordinateSpace::Local);
	customSpline->SetRelativeScale3D(FVector(1.5f, 1.0f, 1.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>BaseCubeRef(TEXT("StaticMesh'/Engine/Basicshapes/Cube'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>BaseCylinderRef(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder'"));

	if (BaseCylinderRef.Object != nullptr)
		BaseCylinder = BaseCylinderRef.Object;
	else
		return;

	if (BaseCubeRef.Object != nullptr)
		BaseCube = BaseCubeRef.Object;
}

// Called when the game starts or when spawned
void ABlackHoleActor::BeginPlay()
{
	Super::BeginPlay();
	BoxCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABlackHoleActor::OnBoxBeginOverlap);
	SetActorTickEnabled(false);
}

float ABlackHoleActor::FlippedSplineTimeValue()
{
	return 4.0f - SplineTime;
}

void ABlackHoleActor::CustomDelayFunction()
{
	AllowCollisions = true;

	for (int i = 0; i < ActorArray.Num(); i++)
	{
		if (IsValid(ActorArray[i]))
			ActorArray[i]->Static_Mesh->SetNotifyRigidBodyCollision(true);
		else
			ActorArray.RemoveAt(i);
	}

	// Remove all items from the list
	ActorArray.Empty();
	EndTime = 0.0f;
	for (int k = 1; k < 4; k++)
	{
		ACubeMesh* lol = GetWorld()->SpawnActor<ACubeMesh>(GetActorLocation() + FVector(0, 0, k * 2000.f), FRotator());
		lol->Static_Mesh->SetStaticMesh(BaseCylinder);
	}

	for (int i = 1; i < 3; i++)
	{
		ACubeMesh* Cube = GetWorld()->SpawnActor<ACubeMesh>(GetActorLocation() + FVector(0, 0, i * 2000.f), FRotator());
		Cube->Static_Mesh->SetStaticMesh(BaseCube);
	}
}

// Called every frame
void ABlackHoleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EndTime < 10.f)
	{
		EndTime += DeltaTime;

		if (SplineTime < 4.0f)
		{
			SplineTime += DeltaTime;
			// Use the location based on the splinetime float variable to get a location on the spline and change the holeposition scene component to always 
			// update
			HolePosition->SetWorldLocation(customSpline->GetLocationAtSplineInputKey(SplineTime, ESplineCoordinateSpace::World));

			DirectionMirror->SetWorldLocation(customSpline->GetLocationAtSplineInputKey(FlippedSplineTimeValue(), ESplineCoordinateSpace::World));

			for (int i = 0; i < ActorArray.Num(); i++)
			{
				if (IsValid(ActorArray[i]) )
				{
					if (ActorArray[i]->NormalDirection)
					{
						FVector VinterpToVec = FVector(FMath::VInterpTo(ActorArray[i]->Static_Mesh->GetComponentLocation(),
							HolePosition->GetComponentLocation(), DeltaTime, 2.0f).X,
							FMath::VInterpTo(ActorArray[i]->Static_Mesh->GetComponentLocation(),
								HolePosition->GetComponentLocation(), DeltaTime, 2.0f).Y, 0.0f);

						ActorArray[i]->Static_Mesh->SetWorldLocation(FVector(VinterpToVec.X, VinterpToVec.Y, HolePosition->GetComponentLocation().Z), false, nullptr, ETeleportType::TeleportPhysics);

						ActorArray[i]->Static_Mesh->AddTorqueInDegrees(FVector(90, 290, 360), FName("None"), true);
					}
					else
					{
						FVector VinterpToVec = FVector(FMath::VInterpTo(ActorArray[i]->Static_Mesh->GetComponentLocation(),
							DirectionMirror->GetComponentLocation(), DeltaTime, 2.0f).X,
							FMath::VInterpTo(ActorArray[i]->Static_Mesh->GetComponentLocation(),
								DirectionMirror->GetComponentLocation(), DeltaTime, 2.0f).Y, 0.0f);

						ActorArray[i]->Static_Mesh->SetWorldLocation(FVector(VinterpToVec.X, VinterpToVec.Y, DirectionMirror->GetComponentLocation().Z), false, nullptr, ETeleportType::TeleportPhysics);
						ActorArray[i]->Static_Mesh->AddTorqueInDegrees(FVector(90, 290, 360), FName("None"), true);
					}
				}
				else
					ActorArray.RemoveAt(i);
			}
		}
		else
			SplineTime = 0.0f;
	}
	else
	{
		AllowCollisions = false;

		for (int i = 0; i < ActorArray.Num(); i++)
		{
			if (IsValid(ActorArray[i]))
			{
				FVector UpHelper = ActorArray[i]->Static_Mesh->GetComponentLocation();
				UpHelper.Z += 300.f;
				FVector dir = customSpline->FindDirectionClosestToWorldLocation(UpHelper, ESplineCoordinateSpace::World);
				dir *= 4000.0f;
				dir += FVector(0.f, 0.f, 2000.f);
				ActorArray[i]->Static_Mesh->AddImpulse(dir, "NONE", true);
				ActorArray[i]->Static_Mesh->AddForce(FVector(0.f, 0.f, 2000.f), "NONE", true);
			}
			else
				ActorArray.RemoveAt(i);
		}

		GetWorld()->GetTimerManager().SetTimer(slightdelay, this, &ABlackHoleActor::CustomDelayFunction, 2.0, false);
		SetActorTickEnabled(false);
	}
}

void ABlackHoleActor::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool From_Sweep, const FHitResult& Sweep_Result)
{
	// If the boolean is true to allow more collisions then proceed
	if (AllowCollisions)
	{
		// If the incoming actor is of some subset of MainBaseActor then continue, otherwise ignore for now
		if (AMainBaseActor* ref = Cast<AMainBaseActor>(OtherActor))
		{
			// If the incoming actor is not already inside the character array then proceed to add it
			if (ActorArray.Find(ref) == INDEX_NONE)
			{
				ActorArray.Add(ref);

				if (ActorArray.Num() > 1)
				{
					LastAddition = !LastAddition;
					ActorArray.Last()->NormalDirection = LastAddition;
				}

				if (!IsActorTickEnabled() )
					SetActorTickEnabled(true);
			}
		}
	}
}

