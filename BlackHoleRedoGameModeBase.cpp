// Copyright Epic Games, Inc. All Rights Reserved.


#include "BlackHoleRedoGameModeBase.h"
#include "MyUserWidget.h"
#include "Blueprint/UserWidget.h"

void ABlackHoleRedoGameModeBase::StartPlay()
{
	Super::StartPlay();

	check(GEngine != nullptr);


	if (IsValid(WidgetClassRef) )
	{
		CreateDefaultWidget();
		spawned_items = 0;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to initialize widget class"));
		return;
	}
	
}

void ABlackHoleRedoGameModeBase::CreateDefaultWidget()
{
	if (WidgetClassRef == nullptr) return;

	myWidget = Cast<UUserWidget>(CreateWidget(GetWorld(), WidgetClassRef) );

	if (myWidget != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("Successful viewport creation!!"));
		ToogleWidgetView();
	}
}

void ABlackHoleRedoGameModeBase::ToogleWidgetView()
{
	if (myWidget->IsInViewport())
	{
		myWidget->RemoveFromViewport();
	}
	else
	{
		myWidget->AddToViewport();
	}
}
