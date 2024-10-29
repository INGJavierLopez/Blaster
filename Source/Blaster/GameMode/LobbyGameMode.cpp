// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "MultiplayerSessionsSubsystem.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Blue,
			FString(TEXT("Entroooooooooooooo"))
		);
	}
	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UMultiplayerSessionsSubsystem* Subsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
		check(Subsystem);
		if (Subsystem->DesiredNumPublicConnections == 2)
		{
			FString PingText = FString::Printf(TEXT("Numero de conexiones publicas de la sesion %d"), Subsystem->DesiredNumPublicConnections);

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					-1,
					15.f,
					FColor::Red,
					PingText
				);
			}
		}
		else
		{
			FString PingText = FString::Printf(TEXT("Otro valor"));

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					-1,
					15.f,
					FColor::Red,
					PingText
				);
			}
		}
		
		if (NumberOfPlayers >= Subsystem->DesiredNumPublicConnections)
		{
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
					World->ServerTravel(FString("/Game/Maps/CaptureTheFlag?listen"));
				}
				else if (MatchType == "Ghost")
				{
					World->ServerTravel(FString("/Game/Maps/GhostMap?listen"));
				}
				
			}
		}
	}
}


void ALobbyGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UMultiplayerSessionsSubsystem* Subsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
		check(Subsystem);
		//FString PingText = FString::Printf(TEXT("Numero de conexiones publicas de la sesion %d"), Subsystem->DesiredNumPublicConnections);
		Prueba(Subsystem->DesiredNumPublicConnections, DeltaSeconds);
	}
}

