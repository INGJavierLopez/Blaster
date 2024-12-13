// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blaster/BlasterTypes/Team.h"
#include "EndGame.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UEndGame : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized();

	virtual void SynchronizeProperties() override;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* FinalWinnerText;

	UPROPERTY(meta = (BindWidget))
	class UImage* TeamWinImage;

	UPROPERTY(meta = (BindWidget))
	class UButton* BackToMenuButton;

	void ShowEndGameResult(ETeam WinnerTeam);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FLinearColor RedTeamColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FLinearColor BlueTeamColor;
};
