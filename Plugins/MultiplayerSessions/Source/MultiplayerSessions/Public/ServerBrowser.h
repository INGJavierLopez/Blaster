// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerBrowser.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UServerBrowser : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(meta = (BindWidget))
	class UButton* BackToMenu_Button;

	UPROPERTY(meta = (BindWidget))
	UButton* SearchButton;

	UPROPERTY(BlueprintReadOnly,meta = (BindWidget))
	class UScrollBox* ServerList_SB;
};
