// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blaster/BlasterTypes/ScoreTabStructures.h"
#include "TeamScoreTab.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UTeamScoreTab : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	class UScrollBox* BlueTeamSB;
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UScrollBox* RedTeamSB;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> TeamScoreSlotClass;

	// Función para ser implementada en Blueprint
	UFUNCTION(BlueprintImplementableEvent, Category = "Team Score Tab")
	void UpdateWidgetTeamScoreTab(const TArray<FScoreSlotInfo>& GroupA, const TArray<FScoreSlotInfo>& GroupB);
	void UpdateTables(const TArray<FScoreSlotInfo>& GroupA, const TArray<FScoreSlotInfo>& GroupB);

};
