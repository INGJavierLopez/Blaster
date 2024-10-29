// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h" // Este archivo contiene la definición de FOnlineSessionSearchResult

#include "ServerBrowserItem.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UServerBrowserItem : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly,meta = (BindWidget))
	class UTextBlock* SessionMatchType;

	UPROPERTY(BlueprintReadOnly,meta = (BindWidget))
	UTextBlock* SessionOwnersName;

	UPROPERTY(BlueprintReadOnly,meta = (BindWidget))
	UTextBlock* NumPlayers;

	UPROPERTY(BlueprintReadOnly,meta = (BindWidget))
	UTextBlock* Ping;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	class FOnlineSessionSearchResult Result;
protected:

	virtual bool Initialize() override;
private:
	UFUNCTION()
	void JoinSession();
};
