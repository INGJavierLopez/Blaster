// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterGameState.h"
#include "Net/UnrealNetwork.h"
#include "Blaster/PlayerState/BlasterPlayerState.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/GameMode/BlasterGameMode.h"
#include "Blaster/GameMode/GhostsGameMode.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

void ABlasterGameState::BeginPlay()
{
	Super::BeginPlay();
	
	ABlasterGameMode* GameMode = Cast<AGhostsGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		bGhostMode = true;
		ServerSetGhostMode(bGhostMode);
	}
	else
	{
		bGhostMode = false;
		ServerSetGhostMode(bGhostMode);
	}
}

void ABlasterGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABlasterGameState, TopScoringPlayers);
	DOREPLIFETIME(ABlasterGameState, RedTeamScore);
	DOREPLIFETIME(ABlasterGameState, BlueTeamScore);
	DOREPLIFETIME(ABlasterGameState, bGhostMode);
}

void ABlasterGameState::UpdateTopScore(ABlasterPlayerState* ScoringPlayer)
{
	if (TopScoringPlayers.Num() == 0)
	{
		TopScoringPlayers.Add(ScoringPlayer);
		TopScore = ScoringPlayer->GetScore();
	}
	else if (ScoringPlayer->GetScore() == TopScore)
	{ 
		TopScoringPlayers.AddUnique(ScoringPlayer);
	}
	else if (ScoringPlayer->GetScore() > TopScore)
	{
		TopScoringPlayers.Empty();
		TopScoringPlayers.AddUnique(ScoringPlayer);
		TopScore = ScoringPlayer->GetScore();
	}
}

void ABlasterGameState::RedTeamScores()
{
	++RedTeamScore;
	ABlasterPlayerController* BPlayer = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (BPlayer)
	{
		BPlayer->SetHUDRedTeamScore(RedTeamScore);
	}
	if (RedTeamScore >= BlueTeam.Num())
	{
		EndRound();
	}
}

void ABlasterGameState::BlueTeamScores()
{
	++BlueTeamScore;
	ABlasterPlayerController* BPlayer = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (BPlayer)
	{
		BPlayer->SetHUDBlueTeamScore(BlueTeamScore);
	}
	if (BlueTeamScore >= RedTeam.Num())
	{
		EndRound();
	}
}

void ABlasterGameState::EndRound()
{
	BlasterGameMode = BlasterGameMode == nullptr ? GetWorld()->GetAuthGameMode<ABlasterGameMode>() : BlasterGameMode;

	GetWorldTimerManager().SetTimer(
		EndRoundTimerHandle,
		BlasterGameMode,
		&ABlasterGameMode::RequestEndRound,
		1.f,
		false
	);
}
void ABlasterGameState::OnRep_RedTeamScore()
{
	ABlasterPlayerController* BPlayer = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (BPlayer)
	{
		BPlayer->SetHUDRedTeamScore(RedTeamScore);
	}
}

void ABlasterGameState::OnRep_BlueTeamScore()
{
	ABlasterPlayerController* BPlayer = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (BPlayer)
	{
		BPlayer->SetHUDBlueTeamScore(BlueTeamScore);
	}
}

void ABlasterGameState::ServerSetGhostMode_Implementation(bool bNewGhostMode)
{
	bGhostMode = bNewGhostMode;
}
