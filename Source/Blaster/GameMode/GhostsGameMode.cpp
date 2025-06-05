// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostsGameMode.h"
#include "BlasterGameMode.h"
#include "Blaster/GameState/BlasterGameState.h"
#include "Blaster/Playerstate/BlasterPlayerState.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"


AGhostsGameMode::AGhostsGameMode()
{
	bGhostGame = true;
	bTeamsMatch = true;
}

void AGhostsGameMode::BeginPlay()
{
	Super::BeginPlay();
	BlasterGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));

}

void AGhostsGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PollInit();
}

void AGhostsGameMode::PlayerEliminated(ABlasterCharacter* ElimmedCharacter, ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController)
{
	ABlasterGameMode::PlayerEliminated(ElimmedCharacter, VictimController, AttackerController); //handle players score

	ABlasterPlayerState* AttackerPlayerState = AttackerController ? Cast<ABlasterPlayerState>(AttackerController->PlayerState) : nullptr;

	if (BlasterGameState && AttackerPlayerState)
	{
		if (AttackerPlayerState->GetTeam() == ETeam::ET_BlueTeam)
		{
			BlasterGameState->BlueTeamScores();
		}
		if (AttackerPlayerState->GetTeam() == ETeam::ET_RedTeam)
		{
			BlasterGameState->RedTeamScores();
		}
	}
}

void AGhostsGameMode::PostLogin(APlayerController* NewPlayer)
{
	ABlasterGameMode::PostLogin(NewPlayer);
	//GameModeSignal();
	BlasterGameState == nullptr ? Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this)) : BlasterGameState;
	if (BlasterGameState == nullptr) return;

	ABlasterPlayerState* BPState = NewPlayer->GetPlayerState<ABlasterPlayerState>();
	if (BPState && BPState->GetTeam() == ETeam::ET_NoTeam)
	{
		if (BlasterGameState->BlueTeam.Num() >= BlasterGameState->RedTeam.Num())
		{
			BlasterGameState->RedTeam.AddUnique(BPState);
			BPState->SetTeam(ETeam::ET_RedTeam);
			BPState->SetGhost(true);
		}
		else
		{
			BlasterGameState->BlueTeam.AddUnique(BPState);
			BPState->SetTeam(ETeam::ET_BlueTeam);
		}
	}

}

void AGhostsGameMode::RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController)
{
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Reset();
		ElimmedCharacter->Destroy();
	}
	if (ElimmedController)
	{
		ABlasterPlayerState* BlasterPlayerState = ElimmedController->GetPlayerState<ABlasterPlayerState>();
	}
	if (ElimmedController)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
		int32 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1);
		RestartPlayerAtPlayerStart(ElimmedController, PlayerStarts[Selection]);
	}
}

void AGhostsGameMode::HandleWaitingToStart(float DeltaTime)
{
	Super::HandleWaitingToStart(DeltaTime);
}

void AGhostsGameMode::HandleMatchHasStarted(float DeltaTime)
{
	ABlasterGameMode::HandleMatchHasStarted(DeltaTime);
	if (BlasterGameState)
	{

		for (auto PState : BlasterGameState->PlayerArray)
		{
			ABlasterPlayerState* BPState = Cast<ABlasterPlayerState>(PState.Get());
			if (BPState && BPState->GetTeam() == ETeam::ET_NoTeam)
			{
				if (BlasterGameState->BlueTeam.Num() >= BlasterGameState->RedTeam.Num())
				{
					BlasterGameState->RedTeam.AddUnique(BPState);
					BPState->SetTeam(ETeam::ET_RedTeam);
					BPState->SetGhost(true);
				}
				else
				{
					BlasterGameState->BlueTeam.AddUnique(BPState);
					BPState->SetTeam(ETeam::ET_BlueTeam);
				}
			}
		}
	}
}

void AGhostsGameMode::HandleNewRound(float DeltaTime)
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
		//RestartPlayers();
		//AssingGroupsToNewTeam();
		//UpdateTeamScoreWidget();

	}
	CountdownTime = NewRoundTime - (GetWorld()->GetTimeSeconds() - StateStartTime);
	if (CountdownTime <= 0.f)
	{


		bFirstRound = false;
		StateStartTime = GetWorld()->GetTimeSeconds();
		SetMatchState(MatchState::MatchInProgress);
	}
}

void AGhostsGameMode::HandleMatchInProgress(float DeltaTime)
{
	Super::HandleMatchInProgress(DeltaTime);
}

void AGhostsGameMode::HandleCooldown(float DeltaTime)
{
	Super::HandleCooldown(DeltaTime);
}

void AGhostsGameMode::HandleEndGame(float DeltaTime)
{
	Super::HandleEndGame(DeltaTime);
}

void AGhostsGameMode::PollInit()
{
	if (BlasterGameState == nullptr)
	{
		BlasterGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));
	}

}
