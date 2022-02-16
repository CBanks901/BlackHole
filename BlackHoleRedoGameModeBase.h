// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BlackHoleRedoGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class BLACKHOLEREDO_API ABlackHoleRedoGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	virtual void StartPlay() override;

protected:
	// Widget object
	UPROPERTY(VisibleInstanceOnly)
	class UUserWidget* myWidget;

	UFUNCTION(BlueprintCallable)
	void CreateDefaultWidget();

	UFUNCTION()
	void ToogleWidgetView();

public:
	// Widget class
	UPROPERTY(EditAnywhere, Category = "Shadow")
	TSubclassOf<class UUserWidget> WidgetClassRef;

	UPROPERTY(BlueprintReadWrite)
	int spawned_items;
};
