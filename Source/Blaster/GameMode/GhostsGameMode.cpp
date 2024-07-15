// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostsGameMode.h"
#include "BlasterGameMode.h"
#include "Blaster/GameState/BlasterGameState.h"
#include "Blaster/Playerstate/BlasterPlayerState.h"

#include "Kismet/GameplayStatics.h"

void AGhostsGameMode::PlayerEliminated(ABlasterCharacter* ElimmedCharacter, ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController)
{
	ABlasterGameMode::PlayerEliminated(ElimmedCharacter, VictimController, AttackerController); //handle players score

}

void AGhostsGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	GameModeSignal();
	ABlasterGameState* BGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));

	if (BGameState)
	{
		for (auto PState : BGameState->PlayerArray)
		{
			ABlasterPlayerState* BPState = NewPlayer->GetPlayerState<ABlasterPlayerState>();
			BPState->SetTeam(BPState->GetTeam());
		}
	}

}

void AGhostsGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
	//GameModeSignal();
	//ABlasterGameMode::HandleMatchHasStarted();
}
