// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h" 

#include "Menu.generated.h"


USTRUCT(BlueprintType)
struct FSessionInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString SessionMatchType;

	UPROPERTY(BlueprintReadOnly)
	FString SessionOwnersName;

	UPROPERTY(BlueprintReadOnly)
	FString Players;

	UPROPERTY(BlueprintReadOnly)
	FString Ping;

	

	// Otros campos relevantes de FOnlineSessionSearchResult
};


/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void MenuSetup(int32 NumberOfPublicConnections = 4, FString TypeOfMatch = FString(TEXT("FreeForAll")), FString LobbyPath = FString(TEXT("/Game/ThirdPersonCPP/Maps/Lobby")));
	
	// Array para almacenar los resultados de la búsqueda de sesiones
	UPROPERTY(BlueprintReadOnly, Category = "Sessions")
	TArray<FSessionInfo> FoundSessions;
	 
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcher;
protected:

	virtual bool Initialize() override;
	virtual void NativeDestruct() override;
	//
	// Callbacks for the custom delegates on the MultiplayerSessionsSubsystem
	//
	UFUNCTION()
	void OnCreateSession(bool bWasSuccessful);
	void OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);
	void OnJoinSession(EOnJoinSessionCompleteResult::Type Result);
	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);
	UFUNCTION()
	void OnStartSession(bool bWasSuccessful);

	UPROPERTY(BlueprintReadOnly,meta = (BindWidget))
	class UCreateGame* CreateGame;

	UPROPERTY(BlueprintReadOnly,meta = (BindWidget))
	class UServerBrowser* ServerBrowser;
	UFUNCTION(BlueprintImplementableEvent)
	void  AddServerItem(FSessionInfo SessionInfo);
	UFUNCTION(BlueprintCallable)
	void GetServerBrowserItem(class UServerBrowserItem* Item);

	class FOnlineSessionSearchResult TempResult;

private:

	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;

	UFUNCTION()
	void HostButtonClicked();

	UFUNCTION()
	void CreateGameButtonClicked();

	UFUNCTION()
	void JoinButtonClicked();

	UFUNCTION()
	void FindSessions();

	void MenuTearDown();

	// The subsystem designed to handle all online session functionality
	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 NumPublicConnections{4};

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString MatchType{TEXT("FreeForAll")};

	FString PathToLobby{TEXT("")};

	UFUNCTION()
	void BackToMenu();
};
