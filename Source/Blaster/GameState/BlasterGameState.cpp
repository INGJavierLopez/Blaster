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
	DOREPLIFETIME(ABlasterGameState, RedTeamRoundScore);
	DOREPLIFETIME(ABlasterGameState, BlueTeamRoundScore);
	DOREPLIFETIME(ABlasterGameState, bGhostMode);
	DOREPLIFETIME(ABlasterGameState, CurrentRound); 
	DOREPLIFETIME(ABlasterGameState, EndRoundType);
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
/// 
///  TEMAS ROUND SCORE
/// 

void ABlasterGameState::ShowEndRoundResult(EEndRoundType NewEndRoundType)
{
	CurrentRound++;
	EndRoundType = NewEndRoundType;
	ABlasterPlayerController* BPlayer = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (BPlayer == nullptr) return;
	if (!HasAuthority()) return;
	switch (EndRoundType)
	{
	case EEndRoundType::ERT_RedTeam:
		RedTeamRoundScores();
		BPlayer->HandleRoundScore(RedTeamRoundScore, BlueTeamRoundScore, CurrentRound, EEndRoundType::ERT_RedTeam);
		break;
	case EEndRoundType::ERT_BlueTeam:
		BlueTeamRoundScores();
		BPlayer->HandleRoundScore(RedTeamRoundScore, BlueTeamRoundScore, CurrentRound, EEndRoundType::ERT_BlueTeam);
		break;
	case EEndRoundType::ERT_Draw:
		BPlayer->HandleRoundScore(RedTeamRoundScore, BlueTeamRoundScore, CurrentRound, EEndRoundType::ERT_Draw);
		break;
	case EEndRoundType::ERT_MAX:
		break;
	default:
		break;
	}
}
void ABlasterGameState::OnRep_EndRoundType()
{
	OnRep_CurrentRound();
}
void ABlasterGameState::OnRep_CurrentRound()
{
	ABlasterPlayerController* BPlayer = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (BPlayer == nullptr) return;
	switch (EndRoundType)
	{
	case EEndRoundType::ERT_RedTeam:
		OnRep_RedTeamRoundScore();
		BPlayer->HandleRoundScore(RedTeamRoundScore, BlueTeamRoundScore, CurrentRound, EEndRoundType::ERT_RedTeam);
		break;
	case EEndRoundType::ERT_BlueTeam:
		OnRep_BlueTeamRoundScore();
		BPlayer->HandleRoundScore(RedTeamRoundScore, BlueTeamRoundScore, CurrentRound, EEndRoundType::ERT_BlueTeam);

		break;
	case EEndRoundType::ERT_Draw:
		BPlayer->HandleRoundScore(RedTeamRoundScore, BlueTeamRoundScore, CurrentRound, EEndRoundType::ERT_Draw);
		break;
	case EEndRoundType::ERT_MAX:
		break;
	default:
		break;
	}
}

void ABlasterGameState::RedTeamRoundScores()
{
	++RedTeamRoundScore;
	/* Local*/
	ABlasterPlayerController* BPlayer = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (BPlayer)
	{
		BPlayer->SetHUDRedTeamRounds(RedTeamRoundScore);
	}

}

void ABlasterGameState::OnRep_RedTeamRoundScore()
{
	ABlasterPlayerController* BPlayer = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (BPlayer)
	{
		BPlayer->SetHUDRedTeamRounds(RedTeamRoundScore);
	}
}



//Esta funcion actualiza el marcador global
void ABlasterGameState::BlueTeamRoundScores()
{
	++BlueTeamRoundScore;
	//UpdateRound();
	/* Local*/
	ABlasterPlayerController* BPlayer = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (BPlayer)
	{
		BPlayer->SetHUDBlueTeamRounds(BlueTeamRoundScore);
	}
}


void ABlasterGameState::OnRep_BlueTeamRoundScore()
{
	ABlasterPlayerController* BPlayer = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (BPlayer)
	{
		BPlayer->SetHUDBlueTeamRounds(BlueTeamRoundScore);

	}
}


void ABlasterGameState::ServerRequestRoundScores_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, TEXT("SERVER REQUEST EJECUTADO"));

	ClientReportRoundScores(RedTeamRoundScore, BlueTeamRoundScore);
}
void ABlasterGameState::ClientReportRoundScores_Implementation(float Red, float Blue)
{
	APlayerController* LocalPC = GetWorld()->GetFirstPlayerController();
	if (APlayerController* BlasterPC = Cast<APlayerController>(LocalPC))
	{
	}

}
/// 
/// TEAM SCORES
/// 

void ABlasterGameState::RedTeamScores()
{
	++RedTeamScore;
	/* Local*/
	ABlasterPlayerController* BPlayer = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (BPlayer)
	{
		BPlayer->SetHUDRedTeamScore(RedTeamScore);
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





void ABlasterGameState::ResetTeamScores()
{
	BlueTeamScore = 0;
	RedTeamScore = 0;
	ABlasterPlayerController* BPlayer = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (BPlayer)
	{
		BPlayer->SetHUDBlueTeamScore(BlueTeamScore);
		BPlayer->SetHUDRedTeamScore(RedTeamScore);
	}
}





void ABlasterGameState::ServerSetGhostMode_Implementation(bool bNewGhostMode)
{
	bGhostMode = bNewGhostMode;
}
