// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlasterGameMode.h"
#include "Blaster/BlasterTypes/Team.h"
#include "TeamsGameMode.generated.h"


/**
 * 
 */
UCLASS()
class BLASTER_API ATeamsGameMode : public ABlasterGameMode
{
	GENERATED_BODY()
public:
	void Debug(bool Active, float DeltaTime);
	ATeamsGameMode();
	virtual void Tick(float DeltaTime) override;
	ETeam CheckIfTeamHasWon();
	void ShowRoundWinner();
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual float CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage) override;
	virtual void PlayerEliminated(class ABlasterCharacter* ElimmedCharacter, class ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController) override;
	virtual void EndGame(bool Teams,ETeam TeamWinner) override;
protected:
	virtual void HandleMatchHasStarted() override;
};
