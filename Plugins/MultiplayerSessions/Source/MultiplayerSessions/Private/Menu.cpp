// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu.h"
#include "CreateGame.h"
#include "ServerBrowser.h"
#include "ServerBrowserItem.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"

void UMenu::MenuSetup(int32 NumberOfPublicConnections, FString TypeOfMatch, FString LobbyPath)
{
	PathToLobby = FString::Printf(TEXT("%s?listen"), *LobbyPath);
	NumPublicConnections = NumberOfPublicConnections;
	MatchType = TypeOfMatch;
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}

	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSession);
		MultiplayerSessionsSubsystem->MultiplayerOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessions);
		MultiplayerSessionsSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);
		MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySession);
		MultiplayerSessionsSubsystem->MultiplayerOnStartSessionComplete.AddDynamic(this, &ThisClass::OnStartSession);
	}
}

bool UMenu::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (HostButton)
	{
		HostButton->OnClicked.AddDynamic(this, &ThisClass::HostButtonClicked);
	}
	if (JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this, &ThisClass::JoinButtonClicked);
	}
	if (CreateGame)
	{
		CreateGame->CrearPartida_Button->OnClicked.AddDynamic(this, &ThisClass::CreateGameButtonClicked);
		CreateGame->BackToMenu_Button->OnClicked.AddDynamic(this, &ThisClass::BackToMenu);
	}
	if (ServerBrowser)
	{
		ServerBrowser->BackToMenu_Button->OnClicked.AddDynamic(this, &ThisClass::BackToMenu);
		ServerBrowser->SearchButton->OnClicked.AddDynamic(this, &ThisClass::FindSessions);
	}
	return true;
}

void UMenu::OnCreateSession(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			World->ServerTravel(PathToLobby);
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Red,
				FString(TEXT("Failed to create session!"))
			);
		}
		HostButton->SetIsEnabled(true);
	}
}

void UMenu::FindSessions()
{
	JoinButton->SetIsEnabled(false);
	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->FindSessions(10000);
	}
}

void UMenu::OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful)
{
	if (MultiplayerSessionsSubsystem == nullptr)
	{

		return;
	}
	/*
	FSessionInfo SessionInfot;
	SessionInfot.SessionMatchType = TEXT("Hola");
	SessionInfot.SessionOwnersName = TEXT("Hola");
	SessionInfot.Players = TEXT("Hola");
	SessionInfot.Ping = TEXT("Hola");
	AddServerItem(SessionInfot);*/
	
	if(SessionResults.Num() == 0)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Red,
				FString(TEXT("No se Encontraron sesiones!!!"))
			);
		}
	}
	if (ServerBrowser->ServerList_SB == nullptr)
	{
		return;
	}
	else
	{
		/*
		ServerBrowser->ServerList_SB->ClearChildren();
		ServerBrowser->ServerList_SB->InsertChildAt()ñ
		FSessionInfo SessionInfot;
		SessionInfot.SessionMatchType = TEXT("Test MT");
		SessionInfot.SessionOwnersName = TEXT("Test OW");
		SessionInfot.Players = TEXT("Test 1/1");
		SessionInfot.Ping = TEXT("Test 11ms");
		AddServerItem(SessionInfot);
		*/
	}
	
	for (auto Result : SessionResults)
	{
		FString SettingsValue;
		Result.Session.SessionSettings.Get(FName("MatchType"), SettingsValue);
		
		FSessionInfo SessionInfo;
		// Ejemplo de agregar un TextBlock al ScrollBox
		FString MatchTypeText = FString::Printf(TEXT("Tipo de Partida: %s"), *SettingsValue);
		FString OwnersNameText = FString::Printf(TEXT("Creada por: %s"), *Result.Session.OwningUserName);
		FString PlayersText = FString::Printf(TEXT("%d/%d Jugadores"), Result.Session.NumOpenPublicConnections, Result.Session.SessionSettings.NumPublicConnections);
		FString PingText = FString::Printf(TEXT("%d ms"), Result.PingInMs);
		SessionInfo.SessionMatchType = MatchTypeText;
		SessionInfo.SessionOwnersName = OwnersNameText;
		SessionInfo.Players = PlayersText;
		SessionInfo.Ping = PingText;
		TempResult = Result;
		AddServerItem(SessionInfo);
		//NewServerBrowserItem->Result = Result;
		return;
		if (SettingsValue == MatchType)
		{
			MultiplayerSessionsSubsystem->JoinSession(Result);
			return;
		}
	}
	if (!bWasSuccessful || SessionResults.Num() == 0)
	{
		JoinButton->SetIsEnabled(true);
	}
	
}

void UMenu::GetServerBrowserItem(UServerBrowserItem* Item)
{
	Item->Result = TempResult;
}

void UMenu::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			FString Address;
			SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);

			APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
			if (PlayerController)
			{
				PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
			}
		}
	}
}

void UMenu::OnDestroySession(bool bWasSuccessful)
{
}

void UMenu::OnStartSession(bool bWasSuccessful)
{
}



void UMenu::HostButtonClicked()
{
	if (WidgetSwitcher)
	{
		WidgetSwitcher->SetActiveWidgetIndex(1);
	}
}

void UMenu::CreateGameButtonClicked()
{
	HostButton->SetIsEnabled(false);
	if (MultiplayerSessionsSubsystem)
	{
		if (CreateGame)
		{
			MultiplayerSessionsSubsystem->CreateSession(CreateGame->NumPublicConnections, CreateGame->MatchType);
		}
	}
}

void UMenu::JoinButtonClicked()
{
	if (WidgetSwitcher)
	{
		WidgetSwitcher->SetActiveWidgetIndex(2);
	}
	return;

	
}



void UMenu::MenuTearDown()
{
	RemoveFromParent();
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
		}
	}
}

void UMenu::BackToMenu()
{
	if (WidgetSwitcher)
	{
		WidgetSwitcher->SetActiveWidgetIndex(0);
	}
}


void UMenu::NativeDestruct()
{
	MenuTearDown();

	Super::NativeDestruct();
}