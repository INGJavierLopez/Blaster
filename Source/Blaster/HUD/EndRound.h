// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndRound.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UEndRound : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* WinnerText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* RoundText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* BlueTeamScoreText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* RedTeamScoreText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NextRoundTimeText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NextRoundText;

	// Esta propiedad se enlazará a la animación en el Blueprint.
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* RedTeamScoreAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* BlueTeamScoreAnim;

	void PlayAnimTeamRoundScores(bool Blue);
};
