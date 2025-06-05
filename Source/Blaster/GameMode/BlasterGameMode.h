// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Blaster/BlasterTypes/Team.h"
#include "BlasterGameMode.generated.h"

namespace MatchState
{
	extern BLASTER_API const FName NewRound; // Prepare Players for the next Round
	extern BLASTER_API const FName MatchInProgress; //Players are in matcn
	extern BLASTER_API const FName Cooldown; // Match duration has been reached. display winner and begin timer.
	extern BLASTER_API const FName EndGame;
}
/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	ABlasterGameMode();
	virtual void Tick(float DeltaTime) override;
	virtual void PlayerEliminated(class ABlasterCharacter* ElimmedCharacter, class ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController);
	virtual void RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController);
	UFUNCTION(BlueprintCallable)
	virtual void ResetCharacters();
	virtual void RestartPlayers();
	virtual void DestroyCurrentCharacters();
	virtual void RequestEndRound();
	void PlayerLeftGame(class ABlasterPlayerState* PlayerLeaving);
	virtual float CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage);

	virtual void EndGame(bool Teams, ETeam TeamWinner);

	UPROPERTY(EditDefaultsOnly)
	float WarmupTime = 10.f;

	UPROPERTY(EditDefaultsOnly)
	float MatchTime = 120.f;

	UPROPERTY(EditDefaultsOnly)
	float CooldownTime = 10.f;

	UPROPERTY(EditDefaultsOnly)
	float NewRoundTime = 3.f;

	float LevelStartingTime = 0.f;

	UPROPERTY(BlueprintReadWrite)
	bool bTeamsMatch = false;

	UPROPERTY(BlueprintReadWrite)
	bool bGhostGame = false;

	
protected:
	virtual void BeginPlay() override;
	virtual void OnMatchStateSet() override;
	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	bool bShowDebugMessages = false;
	virtual void Debug(float DeltaTime);
	// Funciones virtuales para cada estado del juego
	virtual void HandleWaitingToStart(float DeltaTime);
	virtual void HandleMatchHasStarted(float DeltaTime);
	virtual void HandleMatchInProgress(float DeltaTime);
	virtual void HandleNewRound(float DeltaTime);
	virtual void HandleCooldown(float DeltaTime);
	virtual void HandleEndGame(float DeltaTime);
	virtual void CalculateWinners();

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CountdownTime = 0.f;
	float StateStartTime = 0.f;
	float EndMatchTime = 0.f;
	bool bFirstRound = true;
	bool bNewRound = false;
	bool bRoundHasEnded = false;
private:
	class ABlasterGameState* BlasterGameState;

public:
	FORCEINLINE float  GetCountdownTime() { return CountdownTime; }
	FORCEINLINE float  GetStateStartTime() { return StateStartTime; }
	ABlasterGameState* GetBlasterGameState();
};
