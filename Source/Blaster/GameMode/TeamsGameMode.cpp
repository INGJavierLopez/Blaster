// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamsGameMode.h"
#include "Blaster/GameState/BlasterGameState.h"
#include "Blaster/Playerstate/BlasterPlayerState.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/GameInstance/BlasterGameInstance.h"
#include "Blaster/BlasterTypes/ScoreTabStructures.h"
#include "Blaster/GameInstance/BlasterGameInstance.h"

#include "Kismet/GameplayStatics.h"

void ATeamsGameMode::Debug(float DeltaTime)
{
	if (!bShowDebugMessages) return;
	FString Data;
	if (MatchState == MatchState::InProgress)
	{
		Data = FString::Printf(TEXT("MatchState : InProgress: %.2f"), CountdownTime);
		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Blue, Data);
	}
	else if (MatchState == MatchState::Cooldown)
	{
		Data = FString::Printf(TEXT("MatchState : Cooldown: %.2f"), CountdownTime);
		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Blue, Data);

	}
	else if (MatchState == MatchState::NewRound)
	{
		Data = FString::Printf(TEXT("MatchState : NewRound: %.2f"), CountdownTime);
		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Blue, Data);

	}
	return;
	if (GEngine)
	{
		if (GetBlasterGameState())
		{
			FString Text1 = FString::Printf(TEXT("R Team Round Score: %d"), (int32)GetBlasterGameState()->RedTeamRoundScore);
			GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Blue, Text1);
			FString Text2 = FString::Printf(TEXT("B Team Round Score: %d"), (int32)GetBlasterGameState()->BlueTeamRoundScore);
			GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Blue, Text1);
		}

	}

}

ATeamsGameMode::ATeamsGameMode()
{
	bTeamsMatch = true;
}

void ATeamsGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Debug(DeltaTime);
	
}

ETeam ATeamsGameMode::CheckIfTeamHasWon()
{
	//Designacion de rondas
	if (GetBlasterGameState())
	{
		if (GetBlasterGameState()->RedTeamScore > GetBlasterGameState()->BlueTeamScore)  GetBlasterGameState()->RedTeamRoundScores();
		else if (GetBlasterGameState()->BlueTeamScore > GetBlasterGameState()->RedTeamScore)  GetBlasterGameState()->BlueTeamRoundScores();

		if (GetBlasterGameState()->RedTeamRoundScore >= 3.f)
		{
			return ETeam::ET_RedTeam;
		}
		else if (GetBlasterGameState()->BlueTeamRoundScore >= 3.f)
		{
			return ETeam::ET_BlueTeam;
		}
		return ETeam::ET_NoTeam;

	}
	return ETeam::ET_NoTeam;
}

void ATeamsGameMode::ShowRoundWinner()
{
	if (GetBlasterGameState())
	{
		if (GetBlasterGameState()->RedTeamScore == GetBlasterGameState()->BlueTeamScore)
		{
			GetBlasterGameState()->RoundEndResult(EEndRoundType::ERT_Draw);
		}
		else if (GetBlasterGameState()->RedTeamScore > GetBlasterGameState()->BlueTeamScore)
		{
			GetBlasterGameState()->RoundEndResult(EEndRoundType::ERT_RedTeam);
		}
		else
		{
			GetBlasterGameState()->RoundEndResult(EEndRoundType::ERT_BlueTeam);
		}
	}
}

void ATeamsGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	UBlasterGameInstance* BlasterGameInstance = Cast<UBlasterGameInstance>(GetGameInstance());
	if (!BlasterGameInstance) return;

	if (NewPlayer->PlayerState == nullptr) return;

	FString PlayerID = NewPlayer->PlayerState->GetUniqueId()->ToString();
	EGroup Grupo = BlasterGameInstance->GetPlayerGroup(PlayerID);

	ABlasterGameState* BGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));
	if (BGameState == nullptr) return;

	ABlasterPlayerState* BPState = NewPlayer->GetPlayerState<ABlasterPlayerState>();
	if (BPState == nullptr) return;

	if (BlasterGameInstance->PlayerGroupMap.IsEmpty()) // Si no hay grupos definidos
	{
		if (BGameState->BlueTeam.Num() == BGameState->RedTeam.Num())
		{
			BGameState->BlueTeam.AddUnique(BPState);
			BPState->SetTeam(ETeam::ET_BlueTeam);
		}
		else if (BGameState->BlueTeam.Num() < BGameState->RedTeam.Num())
		{
			BGameState->BlueTeam.AddUnique(BPState);
			BPState->SetTeam(ETeam::ET_BlueTeam);
		}
		else
		{
			BGameState->RedTeam.AddUnique(BPState);
			BPState->SetTeam(ETeam::ET_RedTeam);
		}
		return;
	}

	if (Grupo == EGroup::EG_A)
	{
		if (bGroupAHasBlueTeam)
		{
			BGameState->BlueTeam.AddUnique(BPState);
			BPState->SetTeam(ETeam::ET_BlueTeam);
		}
		else
		{
			BGameState->RedTeam.AddUnique(BPState);
			BPState->SetTeam(ETeam::ET_RedTeam);
		}
	}
	else if (Grupo == EGroup::EG_B)
	{
		if (bGroupAHasBlueTeam)
		{
			BGameState->RedTeam.AddUnique(BPState);
			BPState->SetTeam(ETeam::ET_RedTeam);
		}
		else
		{
			BGameState->BlueTeam.AddUnique(BPState);
			BPState->SetTeam(ETeam::ET_BlueTeam);
		}
	}
}



void ATeamsGameMode::Logout(AController* Exiting)
{
	ABlasterGameState* BGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));

	ABlasterPlayerState* BPState = Exiting->GetPlayerState<ABlasterPlayerState>();
	if (BGameState && BPState)
	{
		if (BGameState->RedTeam.Contains(BPState))
		{
			BGameState->RedTeam.Remove(BPState);
		}
		if (BGameState->BlueTeam.Contains(BPState))
		{
			BGameState->BlueTeam.Remove(BPState);
		}
	}
}



void ATeamsGameMode::HandleWaitingToStart(float DeltaTime)
{
	CountdownTime = WarmupTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
	if (CountdownTime <= 0.f)
	{
		StateStartTime = GetWorld()->GetTimeSeconds();
		bFirstRound = true;
		StartMatch();
	}
}

void ATeamsGameMode::HandleMatchHasStarted(float DeltaTime)
{
	bNewRound = true;
	StateStartTime = GetWorld()->GetTimeSeconds();
	SetMatchState(MatchState::NewRound);
}

void ATeamsGameMode::HandleNewRound(float DeltaTime)
{
	if (bNewRound) //Hacer una vez al inicio de una nueva ronda
	{
		bNewRound = false;
		//Reaparecer a todos los jugadores
		//Mostrar el widget de ronda
		//No se pueden mover los panas
		ABlasterGameState* BGameState = GetGameState<ABlasterGameState>();
		if (BGameState)
		{
			BGameState->ResetTeamScores();
		}
		//Reasignar Judores a su posicion de defecto
		RestartPlayers();
		AssingGroupsToNewTeam();
		UpdateTeamScoreWidget();

	}
	CountdownTime = NewRoundTime - (GetWorld()->GetTimeSeconds() - StateStartTime);
	if (CountdownTime <= 0.f)
	{


		bFirstRound = false;
		StateStartTime = GetWorld()->GetTimeSeconds();
		SetMatchState(MatchState::MatchInProgress);
	}
}

void ATeamsGameMode::HandleMatchInProgress(float DeltaTime)
{
	CountdownTime = MatchTime - (GetWorld()->GetTimeSeconds() - StateStartTime);
	if (CountdownTime <= 0.f)
	{
		StateStartTime = GetWorld()->GetTimeSeconds();
		bRoundHasEnded = true;
		//Se determina si se acabo el juego

		//Se verifican los Scores para ver si algun team ha ganado
		ETeam CheckWinnerTeam = CheckIfTeamHasWon();
		if (CheckWinnerTeam == ETeam::ET_NoTeam)
		{
			ShowRoundWinner();
			SetMatchState(MatchState::Cooldown);
		}
		else
		{
			EndGame(true, CheckWinnerTeam);
			DestroyCurrentCharacters();
			SetMatchState(MatchState::EndGame);
		}
	}
}

void ATeamsGameMode::HandleCooldown(float DeltaTime)
{
	if (bRoundHasEnded) //Hacer una vez al termino de una ronda
	{
		bRoundHasEnded = false;
		DestroyCurrentCharacters();
		AssingGroupsToNewTeam();
		ResetCharacters();
		bNewRound = true;

	}
	//Timer para dar paso al siguiente evento
	CountdownTime = CooldownTime - (GetWorld()->GetTimeSeconds() - StateStartTime);
	if (CountdownTime <= 0.f)
	{
		StateStartTime = GetWorld()->GetTimeSeconds();
		//Destruir el pawn de los jugadores
		SetMatchState(MatchState::NewRound);
	}
}

void ATeamsGameMode::HandleEndGame(float DeltaTime)
{

}

void ATeamsGameMode::AssingGroupsToNewTeam()
{
	UBlasterGameInstance* BlasterGameInstance = Cast<UBlasterGameInstance>(GetGameInstance());
	if (!BlasterGameInstance) return;
	ABlasterGameState* BGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));
	if (BGameState == nullptr) return;
	if (BlasterGameInstance->PlayerGroupMap.IsEmpty() && bTeamsInitialized)//No Groups Defined DEBUG Prpouses
	{
		bTeamsInitialized = false;

		for (auto PState : BGameState->PlayerArray)
		{
			ABlasterPlayerState* BPState = Cast<ABlasterPlayerState>(PState.Get());
			if (BGameState->BlueTeam.Num() == BGameState->RedTeam.Num())
			{
				BGameState->BlueTeam.AddUnique(BPState);
				BPState->SetTeam(ETeam::ET_BlueTeam);
			}
			else if (BGameState->BlueTeam.Num() < BGameState->RedTeam.Num())
			{
				BGameState->BlueTeam.AddUnique(BPState);
				BPState->SetTeam(ETeam::ET_BlueTeam);
			}
			else
			{
				BGameState->RedTeam.AddUnique(BPState);
				BPState->SetTeam(ETeam::ET_RedTeam);
			}
		}
		return;
	}
	if (!bSwitchTeams) return;

	BGameState->RedTeam.Empty();
	BGameState->BlueTeam.Empty();

	for (auto PState : BGameState->PlayerArray)
	{
		ABlasterPlayerState* BPState = Cast<ABlasterPlayerState>(PState.Get());
		FString PlayerID = BPState->GetUniqueId()->ToString();

		EGroup Group = BlasterGameInstance->GetPlayerGroup(PlayerID);
		if (Group == EGroup::EG_A)
		{
			if (bGroupAHasBlueTeam)
			{
				BGameState->BlueTeam.AddUnique(BPState);
				BPState->SetTeam(ETeam::ET_BlueTeam);
			}
			else
			{
				BGameState->BlueTeam.AddUnique(BPState);
				BPState->SetTeam(ETeam::ET_RedTeam);
			}

		}
		else if (Group == EGroup::EG_B)
		{
			if (bGroupAHasBlueTeam)
			{
				BGameState->BlueTeam.AddUnique(BPState);
				BPState->SetTeam(ETeam::ET_RedTeam);

			}
			else
			{
				BGameState->BlueTeam.AddUnique(BPState);
				BPState->SetTeam(ETeam::ET_BlueTeam);
			}
		}
	}

	bGroupAHasBlueTeam = !bGroupAHasBlueTeam;
}


void ATeamsGameMode::UpdateTeamScoreWidget()
{
	UBlasterGameInstance* BlasterGameInstance = Cast<UBlasterGameInstance>(GetGameInstance());
	if (!BlasterGameInstance) return;

	TArray<FScoreSlotInfo> GroupA;
	TArray<FScoreSlotInfo> GroupB;

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{

		ABlasterPlayerController* BlasterPlayerController = Cast<ABlasterPlayerController>(*It);
		if (BlasterPlayerController == nullptr) return;

		ABlasterPlayerState* BPlayerState = BlasterPlayerController->GetPlayerState<ABlasterPlayerState>();
		if (BPlayerState == nullptr) return;

		FScoreSlotInfo NewPlayerInfo;
		NewPlayerInfo.PlayerName = BPlayerState->GetPlayerName();
		NewPlayerInfo.Kills = FString::Printf(TEXT("%d"), (int)BPlayerState->GetScore());
		NewPlayerInfo.Deaths = FString::Printf(TEXT("%d"), (int)BPlayerState->GetDefeats());

		FString PlayerID = BPlayerState->GetUniqueId()->ToString();

		if (BlasterGameInstance->PlayerGroupMap.IsEmpty())//No Groups Defined
		{
			if (BPlayerState->GetTeam() == ETeam::ET_BlueTeam) GroupA.Add(NewPlayerInfo);
			else if (BPlayerState->GetTeam() == ETeam::ET_RedTeam) GroupB.Add(NewPlayerInfo);
		}
		else
		{
			EGroup Grupo = BlasterGameInstance->GetPlayerGroup(PlayerID);
			if (Grupo == EGroup::EG_A) GroupA.Add(NewPlayerInfo);
			else if (Grupo == EGroup::EG_B) GroupB.Add(NewPlayerInfo);
		}

		MulticastUpdateScoreTab(GroupA,GroupB);
	}
}

void ATeamsGameMode::MulticastUpdateScoreTab_Implementation(const TArray<FScoreSlotInfo>& GroupA, const TArray<FScoreSlotInfo>& GroupB)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ABlasterPlayerController* BlasterPlayerController = Cast<ABlasterPlayerController>(*It);
		if (BlasterPlayerController)
		{
			BlasterPlayerController->UpdateTeamScoreTab(GroupA, GroupB);
		}
	}
}

float ATeamsGameMode::CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage)
{
	ABlasterPlayerState* AttackerPState = Attacker->GetPlayerState<ABlasterPlayerState>();
	ABlasterPlayerState* VictimPState = Victim->GetPlayerState<ABlasterPlayerState>();
	 
	if (AttackerPState == nullptr || VictimPState == nullptr) return BaseDamage;

	if (VictimPState == AttackerPState)
	{
		return BaseDamage;
	}
	
	if (AttackerPState->GetTeam() == VictimPState->GetTeam())
	{
		return 0;
	}
	
	return BaseDamage;

}

void ATeamsGameMode::PlayerEliminated(ABlasterCharacter* ElimmedCharacter, ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController)
{
	Super::PlayerEliminated(ElimmedCharacter, VictimController, AttackerController);
	UE_LOG(LogTemp, Warning, TEXT("Se llamo player eliminated"));
	ABlasterGameState* BGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));

	ABlasterPlayerState* AttackerPlayerState = AttackerController ? Cast<ABlasterPlayerState>(AttackerController->PlayerState) : nullptr;

	if (BGameState && AttackerPlayerState)
	{
		if (AttackerPlayerState->GetTeam() == ETeam::ET_BlueTeam)
		{
			BGameState->BlueTeamScores();
		}
		if (AttackerPlayerState->GetTeam() == ETeam::ET_RedTeam)
		{
			BGameState->RedTeamScores();
		}
	}
	UpdateTeamScoreWidget();
}

void ATeamsGameMode::EndGame(bool Teams, ETeam TeamWinner)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ABlasterPlayerController* BlasterPlayerController = Cast<ABlasterPlayerController>(*It);
		if (BlasterPlayerController)
		{
			BlasterPlayerController->ShowGameWinner(TeamWinner);
		}
	}
}
