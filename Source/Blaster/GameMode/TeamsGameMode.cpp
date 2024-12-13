// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamsGameMode.h"
#include "Blaster/GameState/BlasterGameState.h"
#include "Blaster/Playerstate/BlasterPlayerState.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Kismet/GameplayStatics.h"

void ATeamsGameMode::Debug(bool Active,float DeltaTime)
{
	if (!Active) return;
	if (MatchState == MatchState::InProgress)
	{
		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Blue, TEXT("MatchState : InProgress"));
	}
	else if (MatchState == MatchState::Cooldown)
	{
		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Blue, TEXT("MatchState : Cooldown"));

	}
	else if (MatchState == MatchState::NewRound)
	{
		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Blue, TEXT("MatchState : NewRound"));

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
	AGameMode::Tick(DeltaTime);
	Debug(true, DeltaTime);
	if (MatchState == MatchState::WaitingToStart)
	{
		UE_LOG(LogTemp, Warning, TEXT("Estoy en Waiting To Start"));
		CountdownTime = WarmupTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			RoundStartTime = GetWorld()->GetTimeSeconds();
			StartMatch();
		}
	}
	else if (MatchState == MatchState::InProgress)
	{
		CountdownTime = MatchTime - (GetWorld()->GetTimeSeconds() + LevelStartingTime - RoundStartTime);
		if (CountdownTime <= 0.f)
		{
			EndMatchTime = GetWorld()->GetTimeSeconds();
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
	else if (MatchState == MatchState::Cooldown)
	{
		if (GetBlasterGameState() == nullptr) return;
		if (bRoundHasEnded) //Hacer una vez al termino de una ronda
		{
			DestroyCurrentCharacters();

		}
		//Timer para dar paso al siguiente evento
		CountdownTime = CooldownTime - (GetWorld()->GetTimeSeconds() - EndMatchTime);
		if (CountdownTime <= 0.f)
		{
			EndMatchTime = GetWorld()->GetTimeSeconds();
			//Destruir el pawn de los jugadores
			SetMatchState(MatchState::NewRound);
		}
	}
	else if (MatchState == MatchState::NewRound)
	{
		if (bNewRound) //Hacer una vez al inicio de una nueva ronda
		{
			bNewRound = false;
			//Reaparecer a todos los jugadores
			ABlasterGameState* BGameState = GetGameState<ABlasterGameState>();
			if (BGameState)
			{
				BGameState->ResetTeamScores();
			}
		}
		//
		CountdownTime = NewRoundTime - (GetWorld()->GetTimeSeconds() - EndMatchTime);
		if (CountdownTime <= 0.f)
		{
			NewRound();
			RoundStartTime = GetWorld()->GetTimeSeconds();
			SetMatchState(MatchState::WaitingToStart);
		}
	}
	else if (MatchState == MatchState::EndGame)
	{
		//El juego se termina, se muestra el ganador y se sacan a todos los jugadores
	}
	
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

	ABlasterGameState* BGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));

	if (BGameState)
	{
		ABlasterPlayerState* BPState = NewPlayer->GetPlayerState<ABlasterPlayerState>();
		if (BPState && BPState->GetTeam() == ETeam::ET_NoTeam)
		{
			if (BGameState->BlueTeam.Num() >= BGameState->RedTeam.Num())
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



void ATeamsGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	ABlasterGameState* BGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));

	if (BGameState)
	{
		
		for (auto PState : BGameState->PlayerArray)
		{
			ABlasterPlayerState* BPState = Cast<ABlasterPlayerState>(PState.Get());
			if (BPState && BPState->GetTeam() == ETeam::ET_NoTeam)
			{
				if (BGameState->BlueTeam.Num() >= BGameState->RedTeam.Num())
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
