// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "MultiplayerSessionsSubsystem.h"
#include "Blaster/PlayerController/GameStartupPlayerController.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
}


void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
	GameStartupPC = Cast<AGameStartupPlayerController>(GetWorld()->GetFirstPlayerController());
	ReadyToTravel = false;
}

void ALobbyGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	GameInstance = GetGameInstance();
	if (StartCounter)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, DeltaSeconds, FColor::Green, TEXT("True"));
	}
	else
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, DeltaSeconds, FColor::Green, TEXT("False"));

	}
	if (GameInstance)
	{
		UMultiplayerSessionsSubsystem* Subsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
		int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
		if (NumberOfPlayers >= Subsystem->DesiredNumPublicConnections && !flag)
		{
			if (SessionState == ESessionState::WaitingForPlayers)
			{
				if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Una vez"));
				SessionState = ESessionState::ReadyToPlay;
				StartCounter = true;
			}
		}
		else
		{
			SessionState = ESessionState::WaitingForPlayers;
			StartCounter = false;
		}
	}
	GameStartupPC == nullptr ? Cast<AGameStartupPlayerController>(GetWorld()->GetFirstPlayerController()) : GameStartupPC;

	if (GameStartupPC == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("PC No Valido"));

	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, DeltaSeconds, FColor::Green, FString::Printf(TEXT("Start Count: %f"), TimeStartCount));
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, DeltaSeconds, FColor::Green, FString::Printf(TEXT("ServerTIme Count : %f"), GameStartupPC->GetServerTime()));

	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, DeltaSeconds, FColor::Green, FString::Printf(TEXT("Remaining Count : %f"), TimeRemainingToTravel));

	}
	if (SessionState == ESessionState::ReadyToPlay && !ReadyToTravel)
	{
		if (GEngine)
		{
			//GEngine->AddOnScreenDebugMessage(-1, DeltaSeconds, FColor::Yellow, TEXT("Ready To Play"));
		}
		if (StartCounter)
		{
			StartCounter = false;
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Pasa Una vez"));
			TimeStartCount = GameStartupPC->GetServerTime();
			TimeRemainingToTravel = TimeStartCount + TimeToTravel;

		}
		if (TimeRemainingToTravel > 0)
		{
			TimeRemainingToTravel = TimeToTravel + TimeStartCount - GameStartupPC->GetServerTime();;
		}
		else
		{
			ReadyToTravel = true;
			StartGame();
		}
	}


}

void ALobbyGameMode::WaitGameMode()
{
	flag = !flag;
}

void ALobbyGameMode::StartGame()
{
	UMultiplayerSessionsSubsystem* Subsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	check(Subsystem);
	UWorld* World = GetWorld();
	if (World)
	{
		bUseSeamlessTravel = true;

		FString MatchType = Subsystem->DesiredMatchType;
		if (MatchType == "FreeForAll")
		{
			World->ServerTravel(FString("/Game/Maps/BlasterMap?listen"));
		}
		else if (MatchType == "Teams")
		{
			World->ServerTravel(FString("/Game/Maps/TeamsMap?listen"));
		}
		else if (MatchType == "CaptureTheFlag")
		{
			World->ServerTravel(FString("/Game/Maps/CaptureTheFlagMap?listen"));
		}
		else if (MatchType == "Ghost")
		{
			World->ServerTravel(FString("/Game/Maps/GhostMap?listen"));
		}

	}
}

