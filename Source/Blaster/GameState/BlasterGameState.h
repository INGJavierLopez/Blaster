// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BlasterGameState.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterGameState : public AGameState
{
	GENERATED_BODY()
public:
	virtual void BeginPlay();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void UpdateTopScore(class ABlasterPlayerState* ScoringPlayer);
	UPROPERTY(Replicated)
	TArray<class ABlasterPlayerState*> TopScoringPlayers;

	/**
	*  Teams
	*/

	void RedTeamScores();
	void BlueTeamScores();
	void ResetTeamScores();
	void RedTeamRoundScores();
	void BlueTeamRoundScores();
	void EndRound();


	UPROPERTY(BlueprintReadWrite)
	TArray<ABlasterPlayerState*> RedTeam;
	UPROPERTY(BlueprintReadWrite)

	TArray<ABlasterPlayerState*> BlueTeam;

	/*
	*  ROUNDS
	*/

	UPROPERTY(ReplicatedUsing = OnRep_RedTeamRoundScore,BlueprintReadOnly)
	float RedTeamRoundScore = 0.f;
	
	UPROPERTY(ReplicatedUsing = OnRep_BlueTeamRoundScore,BlueprintReadOnly)
	float BlueTeamRoundScore = 0.f;

	UFUNCTION()
	void OnRep_RedTeamRoundScore();

	UFUNCTION()
	void OnRep_BlueTeamRoundScore();

	/*
	*  SCORES
	*/
	UPROPERTY(ReplicatedUsing = OnRep_RedTeamScore,BlueprintReadWrite)
	float RedTeamScore = 0.f;

	UPROPERTY(ReplicatedUsing = OnRep_BlueTeamScore, BlueprintReadWrite)
	float BlueTeamScore = 0.f;

	UFUNCTION()
	void OnRep_RedTeamScore();

	UFUNCTION()
	void OnRep_BlueTeamScore();

	/*
	*  GHOST
	*/
	UPROPERTY(Replicated,BlueprintReadWrite)
	bool bGhostMode  = false;
	UFUNCTION(Server,Reliable)
	void ServerSetGhostMode(bool bNewGhostMode);

private:
	float TopScore = 0.f;

	FTimerHandle EndRoundTimerHandle;

	class ABlasterGameMode* BlasterGameMode;

	float MaxRounds = 3.f;
};
