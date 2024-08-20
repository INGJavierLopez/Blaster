// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostsGameMode.h"
#include "BlasterGameMode.h"
#include "Blaster/GameState/BlasterGameState.h"
#include "Blaster/Playerstate/BlasterPlayerState.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Kismet/GameplayStatics.h"

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
	GameModeSignal();
	if (BlasterGameState)
	{
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
}

void AGhostsGameMode::HandleMatchHasStarted()
{
	ABlasterGameMode::HandleMatchHasStarted();
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

void AGhostsGameMode::PollInit()
{
	if (BlasterGameState == nullptr)
	{
		BlasterGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));
	}

}
