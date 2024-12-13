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

void ABlasterGameState::HandleRoundEndResult()
{
	ABlasterPlayerController* BPlayerController = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (BPlayerController == nullptr) return;
	switch (EndRoundType)
	{
	case EEndRoundType::ERT_RedTeam:
		BPlayerController->HandleRoundScore(RedTeamRoundScore, BlueTeamRoundScore, CurrentRound, EEndRoundType::ERT_RedTeam);
		break;
	case EEndRoundType::ERT_BlueTeam:
		BPlayerController->HandleRoundScore(RedTeamRoundScore, BlueTeamRoundScore, CurrentRound, EEndRoundType::ERT_BlueTeam);
		break;
	case EEndRoundType::ERT_Draw:
		BPlayerController->HandleRoundScore(RedTeamRoundScore, BlueTeamRoundScore, CurrentRound, EEndRoundType::ERT_Draw);
		break;
	case EEndRoundType::ERT_MAX:
		break;
	default:
		break;
	}
}

void ABlasterGameState::RoundEndResult(EEndRoundType NewEndRoundType)
{
	CurrentRound++;
	EndRoundType = NewEndRoundType;
	HandleRoundEndResult();
}

void ABlasterGameState::OnRep_EndRoundType()
{
	HandleRoundEndResult();
}
void ABlasterGameState::OnRep_CurrentRound()
{
	HandleRoundEndResult();
}

void ABlasterGameState::RedTeamRoundScores()
{
	++RedTeamRoundScore;
	/* Local*/
	HandleRedTeamRoundScores();
}



void ABlasterGameState::OnRep_RedTeamRoundScore()
{
	HandleRedTeamRoundScores();
}

void ABlasterGameState::HandleRedTeamRoundScores()
{
	ABlasterPlayerController* BPlayerController = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (BPlayerController)
	{
		BPlayerController->BlueprintSignal();
		BPlayerController->SetHUDRedTeamRounds(RedTeamRoundScore);
	}
}

//Esta funcion actualiza el marcador global
void ABlasterGameState::BlueTeamRoundScores()
{
	++BlueTeamRoundScore;

	HandleBlueTeamRoundScores();
}

void ABlasterGameState::OnRep_BlueTeamRoundScore()
{
	HandleBlueTeamRoundScores();
}

void ABlasterGameState::HandleBlueTeamRoundScores()
{
	ABlasterPlayerController* BPlayerController = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (BPlayerController)
	{
		BPlayerController->SetHUDBlueTeamRounds(BlueTeamRoundScore);
	}
}

void ABlasterGameState::ServerRequestRoundScores_Implementation()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, TEXT("SERVER REQUEST EJECUTADO"));

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
	ABlasterPlayerController* BPlayerController = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (BPlayerController)
	{
		BPlayerController->SetHUDRedTeamScore(RedTeamScore);
	}
}



void ABlasterGameState::BlueTeamScores()
{
	++BlueTeamScore;
	ABlasterPlayerController* BPlayerController = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (BPlayerController)
	{
		BPlayerController->SetHUDBlueTeamScore(BlueTeamScore);
	}
}


void ABlasterGameState::OnRep_RedTeamScore()
{
	ABlasterPlayerController* BPlayerController = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (BPlayerController)
	{
		BPlayerController->SetHUDRedTeamScore(RedTeamScore);
	}
}

void ABlasterGameState::OnRep_BlueTeamScore()
{
	ABlasterPlayerController* BPlayerController = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (BPlayerController)
	{
		BPlayerController->SetHUDBlueTeamScore(BlueTeamScore);
	}
}





void ABlasterGameState::ResetTeamScores()
{
	BlueTeamScore = 0;
	RedTeamScore = 0;
	ABlasterPlayerController* BPlayerController = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (BPlayerController)
	{
		BPlayerController->SetHUDBlueTeamScore(BlueTeamScore);
		BPlayerController->SetHUDRedTeamScore(RedTeamScore);
	}
}





void ABlasterGameState::ServerSetGhostMode_Implementation(bool bNewGhostMode)
{
	bGhostMode = bNewGhostMode;
}
