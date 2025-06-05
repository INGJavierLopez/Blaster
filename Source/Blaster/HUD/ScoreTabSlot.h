// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreTabSlot.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UScoreTabSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	class UTextBlock* PlayerNameText;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UTextBlock* KillsText;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UTextBlock* DeathsText;
};
