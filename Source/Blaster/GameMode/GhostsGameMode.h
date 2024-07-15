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
	virtual void PlayerEliminated(class ABlasterCharacter* ElimmedCharacter, class ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	UFUNCTION(BlueprintImplementableEvent)
	void GameModeSignal();
protected:
	virtual void HandleMatchHasStarted() override;
};
