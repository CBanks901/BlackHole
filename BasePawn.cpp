// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Create a dummy root component we can attach things to.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	// Create a camera and a visible object
	
	OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
	OurVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));


	// Attach our camera and visible object to our root component. Offset and rotate the camera.
	OurCamera->SetupAttachment(RootComponent);
	OurCamera->SetRelativeLocation(FVector(-250.0f, 0.0f, 250.0f));
	OurCamera->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	OurVisibleComponent->SetupAttachment(RootComponent);

	
}

// Called when the game starts or when spawned
void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
	if (!CharacterSpeed.IsZero())
	{
		FVector newPosition = GetActorLocation();
		newPosition += GetActorForwardVector() * (CharacterSpeed * DeltaTime);
		SetActorLocation(newPosition);
	}
	
	else if (!MoveRightVec.IsZero())
	{
		SetActorLocation(MoveRightVec);
	}
	if (Pitch > 0.0f)
	{
		if (GetActorRotation().Pitch > -40.0f)
		{
			FRotator someRot = FRotator(GetActorRotation().Pitch - 90.0f, GetActorRotation().Yaw, GetActorRotation().Roll);
			SetActorRotation(FMath::RInterpTo(GetActorRotation(), someRot, DeltaTime, 1.5f));
		}
		else
			GEngine->AddOnScreenDebugMessage(-1, .5f, FColor::Yellow, TEXT("Pitch lower limit reached!!!") ) ;
	}
	else if (Pitch < 0.0f)
	{
		if (GetActorRotation().Pitch < 45.0f)
		{
			FRotator someRot = FRotator(GetActorRotation().Pitch + 90.0f, GetActorRotation().Yaw, GetActorRotation().Roll);
			SetActorRotation(FMath::RInterpTo(GetActorRotation(), someRot, DeltaTime, 1.5f));
		}
		else
			GEngine->AddOnScreenDebugMessage(-1, .5f, FColor::Yellow, TEXT("Pitch upper limit reached!!") );
	}

	if (Yaw > 0.0f)
	{
		FRotator someRot = FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw + 160.0f, GetActorRotation().Roll);
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), someRot, DeltaTime, 1.5f) );
	}

	else if (Yaw < 0.0f)
	{
		FRotator someRot = FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw - 160.0f, GetActorRotation().Roll);
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), someRot, DeltaTime, 1.5f) );
	}
}

// Called to bind functionality to input
void ABasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABasePawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABasePawn::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &ABasePawn::YawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ABasePawn::PitchInput);

	PlayerInputComponent->BindAction("ExitGame", EInputEvent::IE_Pressed, this, &ABasePawn::EndGame);
}

void ABasePawn::MoveForward(float AxisValue)
{
	CharacterSpeed.X = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 1000.f;
	CharacterSpeed.Y = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 1000.f;
}

void ABasePawn::MoveRight(float AxisValue)
{
	MoveRightVec = GetActorRightVector() * FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.f;
	MoveRightVec += GetActorLocation();
}

void ABasePawn::YawInput(float input)
{
	Yaw = FMath::Clamp(input, -1.0f, 1.0f);
}

void ABasePawn::PitchInput(float input)
{
	Pitch = FMath::Clamp(input, -1.0f, 1.0f);
}

void ABasePawn::EndGame()
{
	// Since this is the ownner, we can just use the controller here and cast it as the player controller
	APlayerController* currentcontroller = Cast<APlayerController>(Controller);

	if (currentcontroller != nullptr) currentcontroller->ConsoleCommand("quit");
}

