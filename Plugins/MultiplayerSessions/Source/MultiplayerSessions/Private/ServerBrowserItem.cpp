// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerBrowserItem.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"

bool UServerBrowserItem::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}
	if (JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this, &ThisClass::JoinSession);
	}
	return false;
}

void UServerBrowserItem::JoinSession()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UMultiplayerSessionsSubsystem *MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
		if (MultiplayerSessionsSubsystem)
		{
				MultiplayerSessionsSubsystem->JoinSession(Result);
		}
	}
	
}
