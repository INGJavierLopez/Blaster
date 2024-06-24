// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostsGameMode.h"
#include "Blaster/GameState/BlasterGameState.h"

void AGhostsGameMode::PlayerEliminated(ABlasterCharacter* ElimmedCharacter, ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController)
{
	ABlasterGameMode::PlayerEliminated(ElimmedCharacter, VictimController, AttackerController); //handle players score

}
