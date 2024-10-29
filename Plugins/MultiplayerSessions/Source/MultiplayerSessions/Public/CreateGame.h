// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreateGame.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UCreateGame : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(meta = (BindWidget))
	class UButton* CrearPartida_Button;

	UPROPERTY(meta = (BindWidget))
	UButton* BackToMenu_Button;

	UPROPERTY(meta = (BindWidget))
	class UCheckBox* FreeForAllCheckbox;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* TeamsCheckbox;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* CaptureTheFlagCheckbox;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* GhostCheckbox;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* NOFTextBox;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 NumPublicConnections{ 1 };

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString MatchType{ TEXT("FreeForAll") };

	FString PathToLobby{ TEXT("") };

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void FreeForAllCheckboxCheckStateChanged(bool bIsChecked);
	UFUNCTION()
	void TeamsCheckboxCheckboxCheckStateChanged(bool bIsChecked);
	UFUNCTION()
	void CaptureTheFlagCheckboxCheckboxCheckStateChanged(bool bIsChecked);
	UFUNCTION()
	void GhostCheckboxCheckboxCheckStateChanged(bool bIsChecked);
	
	UFUNCTION()
	void OnTextChanged(const FText& Text);

	UFUNCTION()
	void OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
};
