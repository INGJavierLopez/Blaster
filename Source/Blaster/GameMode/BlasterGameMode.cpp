// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterGameMode.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Blaster/PlayerState/BlasterPlayerState.h"
#include "Blaster/GameState/BlasterGameState.h"

namespace MatchState
{
	const FName NewRound = FName("NewRound"); 
	const FName MatchInProgress = FName("MatchInProgress");
	const FName Cooldown = FName("Cooldown"); 
	const FName EndGame = FName("EndGame");
}

ABlasterGameMode::ABlasterGameMode()
{
	bDelayedStart = true;
}

void ABlasterGameMode::Debug(float DeltaTime)
{
	if (!bShowDebugMessages) return;
	FString Data;
	if (MatchState == MatchState::MatchInProgress)
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
}



void ABlasterGameMode::BeginPlay()
{
	Super::BeginPlay();

	LevelStartingTime = GetWorld()->GetTimeSeconds();
}

void ABlasterGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Debug(DeltaTime);
	if (MatchState == MatchState::WaitingToStart)
	{
		HandleWaitingToStart(DeltaTime);
	}

	else if (MatchState == MatchState::MatchInProgress)
	{
		HandleMatchInProgress(DeltaTime);
	}
	else if (MatchState == MatchState::NewRound)
	{
		HandleNewRound(DeltaTime);
	}
	else if (MatchState == MatchState::InProgress)
	{
		HandleMatchHasStarted(DeltaTime);
	}
	else if (MatchState == MatchState::Cooldown)
	{
		HandleCooldown(DeltaTime);
	}
	else if (MatchState == MatchState::EndGame)
	{
		HandleEndGame(DeltaTime);
	}
}

void ABlasterGameMode::HandleWaitingToStart(float DeltaTime)
{
	CountdownTime = WarmupTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
	if (CountdownTime <= 0.f)
	{
		//SetMatchState(MatchState::WaitingToStart);
		StateStartTime = GetWorld()->GetTimeSeconds();
		bFirstRound = true;
		StartMatch();
	}
}

void ABlasterGameMode::HandleMatchHasStarted(float DeltaTime)
{
	bNewRound = true;
	StateStartTime = GetWorld()->GetTimeSeconds();
	SetMatchState(MatchState::NewRound);
}

void ABlasterGameMode::HandleNewRound(float DeltaTime)
{
	//Funcion que se llama cuando inicia el juego
	if (bFirstRound)
	{
		bFirstRound = false;

	}
	CountdownTime = NewRoundTime - (GetWorld()->GetTimeSeconds() - StateStartTime);
	if (CountdownTime <= 0.f)
	{
		StateStartTime = GetWorld()->GetTimeSeconds();
		bNewRound = true;

		SetMatchState(MatchState::MatchInProgress);
		RestartPlayers();
	}
}

void ABlasterGameMode::HandleMatchInProgress(float DeltaTime)
{
	//Se ejecuta cada nuevo inicio de ronda
	if (bNewRound)
	{
		bNewRound = false;

	}
	CountdownTime = MatchTime - (GetWorld()->GetTimeSeconds() - StateStartTime);
	if (CountdownTime <= 0.f)
	{
		SetMatchState(MatchState::Cooldown);
	}
}



void ABlasterGameMode::HandleCooldown(float DeltaTime)
{
	CountdownTime = CooldownTime - (GetWorld()->GetTimeSeconds() - StateStartTime);
	if (CountdownTime <= 0.f)
	{
		StateStartTime = GetWorld()->GetTimeSeconds();

		SetMatchState(MatchState::EndGame);

	}
}

void ABlasterGameMode::HandleEndGame(float DeltaTime)
{
	CalculateWinners();
}

/**
* Determinar el ganador de la Partida.
*/
void ABlasterGameMode::CalculateWinners()
{
	BlasterGameState == nullptr ? GetGameState<ABlasterGameState>() : BlasterGameState;
	if (BlasterGameState)
	{
		// Estructura para guardar PlayerState + Score
		TArray<TPair<ABlasterPlayerState*, float>> PlayerScores;

		// Recorrer todos los PlayerStates
		for (APlayerState* PlayerState : BlasterGameState->PlayerArray)
		{
			ABlasterPlayerState* BlasterPlayerState = Cast<ABlasterPlayerState>(PlayerState);
			if (BlasterPlayerState)
			{
				float Score = BlasterPlayerState->GetScore();
				PlayerScores.Add(TPair<ABlasterPlayerState*, float>(BlasterPlayerState, Score));
			}
		}

		// Ordenar de mayor a menor score
		PlayerScores.Sort([](const TPair<ABlasterPlayerState*, float>& A, const TPair<ABlasterPlayerState*, float>& B)
			{
				return A.Value > B.Value;
			});

		// Variables para guardar los 3 mejores lugares
		TArray<ABlasterPlayerState*> FirstPlace;
		TArray<ABlasterPlayerState*> SecondPlace;
		TArray<ABlasterPlayerState*> ThirdPlace;

		float FirstScore = -1.f;
		float SecondScore = -1.f;
		float ThirdScore = -1.f;

		// Determinar lugares
		for (const TPair<ABlasterPlayerState*, float>& Pair : PlayerScores)
		{
			float Score = Pair.Value;
			ABlasterPlayerState* Player = Pair.Key;

			if (FirstPlace.Num() == 0)
			{
				FirstScore = Score;
				FirstPlace.Add(Player);
			}
			else if (FMath::IsNearlyEqual(Score, FirstScore))
			{
				FirstPlace.Add(Player);
			}
			else if (SecondPlace.Num() == 0)
			{
				SecondScore = Score;
				SecondPlace.Add(Player);
			}
			else if (FMath::IsNearlyEqual(Score, SecondScore))
			{
				SecondPlace.Add(Player);
			}
			else if (ThirdPlace.Num() == 0)
			{
				ThirdScore = Score;
				ThirdPlace.Add(Player);
			}
			else if (FMath::IsNearlyEqual(Score, ThirdScore))
			{
				ThirdPlace.Add(Player);
			}

			// Si ya tenemos los 3 lugares, podemos salir
			if (ThirdPlace.Num() > 0 && FirstPlace.Num() > 0 && SecondPlace.Num() > 0)
			{
				break;
			}
		}

	}
}


void ABlasterGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();
	//Itera por todos los jugadores y les avisa del Cambio del game State
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ABlasterPlayerController* BlasterPlayerController = Cast<ABlasterPlayerController>(*It);
		if (BlasterPlayerController)
		{
			BlasterPlayerController->OnMatchStateSet(MatchState, bTeamsMatch);
		}
	}
}



ABlasterGameState* ABlasterGameMode::GetBlasterGameState()
{
	return GetGameState<ABlasterGameState>();
}

float ABlasterGameMode::CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage)
{
	return BaseDamage;
}
/*
* Color false = blue, true = red
*/
void ABlasterGameMode::EndGame(bool Teams, ETeam TeamWinner)
{

}

void ABlasterGameMode::PlayerEliminated(class ABlasterCharacter* ElimmedCharacter, class ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController)
{
	if (AttackerController == nullptr || AttackerController->PlayerState == nullptr) return;
	if (VictimController == nullptr || VictimController->PlayerState == nullptr) return;

	ABlasterPlayerState* AttackerPlayerState = AttackerController ? Cast<ABlasterPlayerState>(AttackerController->PlayerState) : nullptr;
	ABlasterPlayerState* VictimPlayerState = VictimController ? Cast<ABlasterPlayerState>(VictimController->PlayerState) : nullptr;

	BlasterGameState = BlasterGameState == nullptr ? GetGameState<ABlasterGameState>() : BlasterGameState; // check GameState

	if (AttackerPlayerState && AttackerPlayerState != VictimPlayerState && BlasterGameState) 
	{
		TArray<ABlasterPlayerState*> PlayersCurrentlyInTheLead;
		for (auto LeadPlayer : BlasterGameState->TopScoringPlayers)
		{
			PlayersCurrentlyInTheLead.Add(LeadPlayer);
		}
		AttackerPlayerState->AddToScore(1.f);
		BlasterGameState->UpdateTopScore(AttackerPlayerState);
		if (BlasterGameState->TopScoringPlayers.Contains(AttackerPlayerState))
		{
			ABlasterCharacter* Leader = Cast<ABlasterCharacter>(AttackerPlayerState->GetPawn());
			if (Leader)
			{
				Leader->MulticastGainedTheLead();
			}
		}

		for (int32 i = 0; i < PlayersCurrentlyInTheLead.Num(); i++)
		{
			if (!BlasterGameState->TopScoringPlayers.Contains(PlayersCurrentlyInTheLead[i]))
			{
				ABlasterCharacter* Loser = Cast<ABlasterCharacter>(PlayersCurrentlyInTheLead[i]->GetPawn());
				if (Loser)
				{
					Loser->MulticastLostTheLead();
				}
			}
		}
	}
	if (VictimPlayerState)
	{
		VictimPlayerState->AddToDefeats(1);
	}

	if (ElimmedCharacter)
	{
		ElimmedCharacter->Elim(false);
	}

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ABlasterPlayerController* BlasterPlayer = Cast<ABlasterPlayerController>(*It);
		if (BlasterPlayer && AttackerPlayerState && VictimPlayerState)
		{
			BlasterPlayer->BroadcastElim(AttackerPlayerState, VictimPlayerState);
		}
	}
}

void ABlasterGameMode::RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController)
{
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Reset();
		ElimmedCharacter->Destroy();
	}
	if (MatchState != MatchState::MatchInProgress) return;
	if (ElimmedController)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
		if (PlayerStarts.Num() == 0) return;
		int32 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1);
		RestartPlayerAtPlayerStart(ElimmedController, PlayerStarts[Selection]);
	}
}

void ABlasterGameMode::ResetCharacters()
{
	//Tiene que estar el juego terminado, iniciado desde el principio valla

	if (BlasterGameState) 
	{
		for (auto PState : BlasterGameState->PlayerArray)
		{
			ABlasterPlayerState* BPState = Cast<ABlasterPlayerState>(PState.Get());

			ABlasterPlayerController* BPController = Cast<ABlasterPlayerController>(BPState->GetPlayerController());
			if (BPController)
			{
				TArray<AActor*> PlayerStarts;
				UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
				int32 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1);
				RestartPlayerAtPlayerStart(BPController, PlayerStarts[Selection]);
				BPController->SetPlayerGameplayMovement(true);
			}
		}
	}
}
/**
 * Reinicia a todos los jugadores en posiciones aleatorias de PlayerStart.
 * También desactiva el movimiento de gameplay en el PlayerController.
 */
void ABlasterGameMode::RestartPlayers()
{
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
	if (PlayerStarts.IsEmpty()) return;
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ABlasterPlayerController* BlasterPlayerController = Cast<ABlasterPlayerController>(*It);
		if (BlasterPlayerController)
		{
			int32 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1);
			RestartPlayerAtPlayerStart(BlasterPlayerController, PlayerStarts[Selection]);
			//BlasterPlayerController->SetPlayerGameplayMovement(false);
		}

	}
}

void ABlasterGameMode::DestroyCurrentCharacters()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ABlasterPlayerController* BlasterPlayerController = Cast<ABlasterPlayerController>(*It);
		if (BlasterPlayerController)
		{
			ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(BlasterPlayerController->GetPawn());
			if (BlasterCharacter)
			{
				//BlasterCharacter->Reset();
				BlasterCharacter->Destroy();
			}
		}
	}
}

void ABlasterGameMode::RequestEndRound()
{
	EndMatchTime = GetWorld()->GetTimeSeconds();
	SetMatchState(MatchState::Cooldown);

}

void ABlasterGameMode::PlayerLeftGame(ABlasterPlayerState* PlayerLeaving)
{
	if (PlayerLeaving == nullptr) return;

	if (BlasterGameState && BlasterGameState->TopScoringPlayers.Contains(PlayerLeaving))
	{
		BlasterGameState->TopScoringPlayers.Remove(PlayerLeaving);
	}
	ABlasterCharacter* CharacterLeaving = Cast<ABlasterCharacter>(PlayerLeaving->GetPawn());
	if (CharacterLeaving)
	{
		CharacterLeaving->Elim(true);
	}
}