// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectTeam.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API USelectTeam : public UUserWidget
{
	GENERATED_BODY()
public:
	// Función para ser implementada en Blueprint
	UFUNCTION(BlueprintImplementableEvent, Category = "Team Score Tab")
	void UpdateTeamWidget(const TArray<FScoreSlotInfo>& GroupA, const TArray<FScoreSlotInfo>& GroupB);

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinGroupA;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinGroupB;
protected:
	virtual void NativeConstruct() override;
private:
	UFUNCTION()
	void JoinGroupACkicked();
	UFUNCTION()
	void JoinGroupBCkicked();
};
