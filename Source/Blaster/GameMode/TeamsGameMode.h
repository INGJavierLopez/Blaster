// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlasterGameMode.h"
#include "Blaster/BlasterTypes/Team.h"
#include "Blaster/BlasterTypes/ScoreTabStructures.h"
#include "TeamsGameMode.generated.h"


/**
 * 
 */
UCLASS()
class BLASTER_API ATeamsGameMode : public ABlasterGameMode
{
	GENERATED_BODY()
public:
	ATeamsGameMode();
	virtual void Tick(float DeltaTime) override;
	ETeam CheckIfTeamHasWon();
	void ShowRoundWinner();
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual float CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage) override;
	virtual void PlayerEliminated(class ABlasterCharacter* ElimmedCharacter, class ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController) override;
	virtual void EndGame(bool Teams, ETeam TeamWinner) override;

protected:
	virtual void HandleWaitingToStart(float DeltaTime) override;
	virtual void HandleMatchHasStarted(float DeltaTime) override;
	virtual void HandleNewRound(float DeltaTime) override;
	virtual void HandleMatchInProgress(float DeltaTime) override;
	virtual void HandleCooldown(float DeltaTime) override;
	virtual void HandleEndGame(float DeltaTime) override;

	void AssingGroupsToNewTeam();
	virtual void Debug(float DeltaTime) override;
	bool bTeamsInitialized = true;
private:
	UPROPERTY(EditDefaultsOnly)
	bool bSwitchTeams = false;

	bool bGroupAHasBlueTeam = true;
	UFUNCTION(NetMulticast, Reliable)
	void MulticastUpdateScoreTab(const TArray<FScoreSlotInfo>& GroupA, const TArray<FScoreSlotInfo>& GroupB);

	void UpdateTeamScoreWidget();
};
