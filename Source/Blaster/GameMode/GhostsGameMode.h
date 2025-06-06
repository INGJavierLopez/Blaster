// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TeamsGameMode.h"
#include "GhostsGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API AGhostsGameMode : public ATeamsGameMode
{
	GENERATED_BODY()
public:
	AGhostsGameMode();
	virtual void BeginPlay();
	virtual void Tick(float DeltaTime) override;
	virtual void PlayerEliminated(class ABlasterCharacter* ElimmedCharacter, class ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	UFUNCTION(BlueprintImplementableEvent)
	void GameModeSignal();

	virtual void RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController);

protected:
	virtual void HandleWaitingToStart(float DeltaTime) override;
	virtual void HandleMatchHasStarted(float DeltaTime) override;
	virtual void HandleNewRound(float DeltaTime) override;
	virtual void HandleMatchInProgress(float DeltaTime) override;
	virtual void HandleCooldown(float DeltaTime) override;
	virtual void HandleEndGame(float DeltaTime) override;
private:
	class ABlasterGameState* BlasterGameState;

	void PollInit();
};
