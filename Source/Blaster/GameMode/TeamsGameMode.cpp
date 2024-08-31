// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamsGameMode.h"
#include "Blaster/GameState/BlasterGameState.h"
#include "Blaster/GameState/BlasterGameState.h"
#include "Blaster/Playerstate/BlasterPlayerState.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Kismet/GameplayStatics.h"

ATeamsGameMode::ATeamsGameMode()
{
	bTeamsMatch = true;
}

void ATeamsGameMode::Tick(float DeltaTime)
{
	AGameMode::Tick(DeltaTime);

	if (MatchState == MatchState::WaitingToStart)
	{
		UE_LOG(LogTemp, Warning, TEXT("Estoy en Waiting To Start"));
		CountdownTime = WarmupTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (/* CountdownTime <= 0.f  &&*/ false)
		{
			StartMatch();
		}
	}
	else if (MatchState == MatchState::InProgress)
	{
		CountdownTime = MatchTime - (GetWorld()->GetTimeSeconds() + LevelStartingTime - RoundStartTime);
		if (CountdownTime <= 0.f)
		{
			EndMatchTime = GetWorld()->GetTimeSeconds();
			SetMatchState(MatchState::Cooldown);
		}
	}
	else if (MatchState == MatchState::Cooldown)
	{
		CountdownTime = CooldownTime - (GetWorld()->GetTimeSeconds() - EndMatchTime);
		if (CountdownTime <= 0.f)
		{
			EndMatchTime = GetWorld()->GetTimeSeconds();
			bNewRound = true;
			SetMatchState(MatchState::NewRound);

			return;
			ABlasterGameState* BGameState = GetGameState<ABlasterGameState>();
			if(BGameState)
			{
				if (BGameState->RedTeamRoundScore >= 3)
				{
					EndGame(true, true);
					SetMatchState(MatchState::EndGame);

				}
				else if (BGameState->BlueTeamRoundScore >= 3)
				{
					EndGame(true, false);
					SetMatchState(MatchState::EndGame);
				}
				else
				{
					SetMatchState(MatchState::NewRound);
				}
			}
		}
	}
	else if (MatchState == MatchState::NewRound)
	{
		if (bNewRound)
		{
			bNewRound = false;
			//Itera por todos los jugadores y les avisa del Cambio del game State
			for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
			{
				ABlasterPlayerController* BlasterPlayerController = Cast<ABlasterPlayerController>(*It);
				if (BlasterPlayerController)
				{
					ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(BlasterPlayerController->GetPawn());
					BlasterCharacter->Elim(false);
				}

			}
			ABlasterGameState* BGameState = GetGameState<ABlasterGameState>();
			if (BGameState)
			{
				if (BGameState->RedTeamScore == BGameState->BlueTeamScore)
				{
					//mensaje de empate de ronda
				}
				else if (BGameState->RedTeamScore > BGameState->BlueTeamScore)
				{
					BGameState->RedTeamRoundScores();
				}
				else 
				{
					BGameState->BlueTeamRoundScores();
				}
				//Mostrar MEnsaje en pantalla
			}
		}
		//
		CountdownTime = NewRoundTime - (GetWorld()->GetTimeSeconds() - EndMatchTime);
		if (CountdownTime <= 0.f)
		{
			ABlasterGameState* BGameState = GetGameState<ABlasterGameState>();
			if (BGameState)
			{
				BGameState->ResetTeamScores();
			}
			RoundStartTime = GetWorld()->GetTimeSeconds();
			SetMatchState(MatchState::WaitingToStart);
		}
	}
	else if (MatchState == MatchState::EndGame)
	{
		//El juego se termina, se muestra el ganador y se sacan a todos los gudaroes
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

void ATeamsGameMode::EndGame(bool Teams, bool Color)
{

}
