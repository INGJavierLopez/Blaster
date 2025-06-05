// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "MultiplayerSessionsSubsystem.h"
#include "Blaster/PlayerController/GameStartupPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "Blaster/GameInstance/BlasterGameInstance.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	UBlasterGameInstance* BlasterGameInstance = Cast<UBlasterGameInstance>(GetGameInstance());
	if (!BlasterGameInstance) return;
	if (NewPlayer && NewPlayer->PlayerState)
	{
		FString NetID = NewPlayer->PlayerState->GetUniqueId()->ToString();

		// Lógica para asignar el grupo
		EGroup Group = EGroup::EG_A;

		// Asignar al grupo
		BlasterGameInstance->AssignPlayerToGroup(NetID, Group);
		UpdateTeamsTable();
	}
	else
	{
		PendingPlayerControllers.AddUnique(NewPlayer);
	}
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
	Debug(DeltaSeconds);
	CheckLocalPlayers(DeltaSeconds);
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

void ALobbyGameMode::Debug(float DeltaSeconds)
{
	// Enviar los datos actualizados a los PlayerControllers
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		AGameStartupPlayerController* BlasterPlayerController = Cast<AGameStartupPlayerController>(*It);
		if (BlasterPlayerController)
		{
			if (BlasterPlayerController->PlayerState)
			{
				FString Data = BlasterPlayerController->PlayerState->GetPlayerName();
				if (GEngine) GEngine->AddOnScreenDebugMessage(-1, DeltaSeconds, FColor::Orange, Data);
				FString Data2 = BlasterPlayerController->PlayerState->GetUniqueId()->ToString();
				if (GEngine) GEngine->AddOnScreenDebugMessage(-1, DeltaSeconds, FColor::Orange, Data2);

			}
		}
	}
}

void ALobbyGameMode::WaitGameMode()
{
	flag = !flag;
}

void ALobbyGameMode::AssingPlayerToGroup(APlayerController* PlayerController,EGroup NewGroup)
{
	UBlasterGameInstance* BlasterGameInstance = Cast<UBlasterGameInstance>(GetGameInstance());
	if (!BlasterGameInstance) return;
	if (PlayerController && PlayerController->PlayerState)
	{
		FString NetID = PlayerController->PlayerState->GetUniqueId()->ToString();
		BlasterGameInstance->AssignPlayerToGroup(NetID, NewGroup);
	}

	UpdateTeamsTable();
	
}
AGameStartupPlayerController* ALobbyGameMode::GetPlayerControllerByNetID(UWorld* World, const FString& NetID)
{
	for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
	{
		AGameStartupPlayerController* PlayerController = Cast<AGameStartupPlayerController>(It->Get());
		if (PlayerController && PlayerController->PlayerState &&
			PlayerController->PlayerState->GetUniqueId().IsValid() &&
			PlayerController->PlayerState->GetUniqueId()->ToString() == NetID)
		{
			return PlayerController;
		}
	}

	return nullptr;
}

void ALobbyGameMode::UpdateTeamsTable()
{
	UBlasterGameInstance* BlasterGameInstance = Cast<UBlasterGameInstance>(GetGameInstance());
	if (!BlasterGameInstance) return;

	// Crear los arreglos para almacenar los jugadores según su grupo
	TArray<FScoreSlotInfo> GroupA;
	TArray<FScoreSlotInfo> GroupB;

	// Iterar directamente sobre el mapa PlayerGroupMap
	for (const TPair<FString, EGroup>& Entry : BlasterGameInstance->PlayerGroupMap)
	{
		const FString& PlayerControllerID = Entry.Key;
		const EGroup& PlayerGroup = Entry.Value;

		// Buscar el PlayerController asociado
		AGameStartupPlayerController* PlayerController = GetPlayerControllerByNetID(GetWorld(), PlayerControllerID);
		if (!PlayerController) continue;

		// Crear la estructura FScoreSlotInfo para este jugador
		FScoreSlotInfo NewPlayerInfo;
		if (PlayerController->PlayerState)
		{
			NewPlayerInfo.PlayerName = PlayerController->PlayerState->GetPlayerName();

		}

		// Asignar al grupo correspondiente
		if (PlayerGroup == EGroup::EG_A)
		{
			GroupA.Add(NewPlayerInfo);
		}
		else if (PlayerGroup == EGroup::EG_B)
		{
			GroupB.Add(NewPlayerInfo);
		}
	}

	// Enviar los datos actualizados a los PlayerControllers
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		AGameStartupPlayerController* BlasterPlayerController = Cast<AGameStartupPlayerController>(*It);
		if (BlasterPlayerController)
		{
			BlasterPlayerController->UpdateTeamScoreTab(GroupA, GroupB);
		}
	}
}

void ALobbyGameMode::CheckLocalPlayers(float DeltaTime)
{
	UBlasterGameInstance* BlasterGameInstance = Cast<UBlasterGameInstance>(GetGameInstance());
	if (PendingPlayerControllers.IsEmpty()) return;
	FString Data = FString::Printf(TEXT("Total: %d"), PendingPlayerControllers.Num());
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Black, Data);

	for(APlayerController* PlayerController : PendingPlayerControllers)
	{
		if (PlayerController->PlayerState)
		{
			FString NetID = PlayerController->PlayerState->GetUniqueId()->ToString();
			// Lógica para asignar el grupo
			EGroup Group = EGroup::EG_B;

			// Asignar al grupo
			BlasterGameInstance->AssignPlayerToGroup(NetID, Group);
			PendingPlayerControllers.Remove(PlayerController);
			UpdateTeamsTable();
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Black, FString("Se Corrigio un PlayerState"));
		}
		else
		{
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Black, FString("No queda el LOcal"));

		}
	}
}

void ALobbyGameMode::StartGame()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		AGameStartupPlayerController* GSPlayerController = Cast<AGameStartupPlayerController>(*It);
		if (GSPlayerController == nullptr) return;
		GSPlayerController->ClientRemoveCurrentHUD();

	}

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

