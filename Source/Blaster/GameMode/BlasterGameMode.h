// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BlasterGameMode.generated.h"

namespace MatchState
{
	extern BLASTER_API const FName NewRound;
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
	virtual void RequestEndRound();
	void PlayerLeftGame(class ABlasterPlayerState* PlayerLeaving);
	virtual float CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage);

	virtual void EndGame(bool Teams, bool Color);

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

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CountdownTime = 0.f;

	float EndMatchTime = 0.f;

	float RoundStartTime = 0.f;

	bool bNewRound = false;
protected:
	virtual void BeginPlay() override;
	virtual void OnMatchStateSet() override;

private:
	class ABlasterGameState* BlasterGameState;

public:
	FORCEINLINE float  GetCountdownTime() { return CountdownTime; }
	FORCEINLINE ABlasterGameState* GetBlasterGameState() { return BlasterGameState; }
};
