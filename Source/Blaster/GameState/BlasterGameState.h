// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BlasterGameState.generated.h"

UENUM(BlueprintType)
enum class EEndRoundType : uint8
{
	ERT_RedTeam UMETA(DisplayName = "Red Team Win"),
	ERT_BlueTeam UMETA(DisplayName = "Blue Team Win"),
	ERT_Draw UMETA(DisplayName = "Draw"),
	ERT_MAX UMETA(DisplayName = "DefaultMAX")
};

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
	void ShowEndRoundResult(EEndRoundType EndRoundType);
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

	UFUNCTION(Server, Reliable)
	void ServerRequestRoundScores();
	// RPC Reports the current server time to the client in response to ServerRequestTime
	UFUNCTION(Client, Reliable)
	void ClientReportRoundScores(float Red, float Blue);

	/*
	*  TEAM SCORES
	*/

	UFUNCTION()
	void OnRep_CurrentRound();
	UPROPERTY(ReplicatedUsing = OnRep_CurrentRound, BlueprintReadWrite)
	int32 CurrentRound = 1.f; 

	UFUNCTION()
	void OnRep_RedTeamScore();
	UPROPERTY(ReplicatedUsing = OnRep_RedTeamScore,BlueprintReadWrite)
	float RedTeamScore = 0.f;

	UFUNCTION()
	void OnRep_BlueTeamScore();
	UPROPERTY(ReplicatedUsing = OnRep_BlueTeamScore, BlueprintReadWrite)
	float BlueTeamScore = 0.f;
	
	UFUNCTION()
	void OnRep_EndRoundType();
	UPROPERTY(ReplicatedUsing = OnRep_EndRoundType, BlueprintReadWrite)
	EEndRoundType EndRoundType;
	

	/*
	*  GHOST
	*/
	UPROPERTY(Replicated,BlueprintReadWrite)
	bool bGhostMode  = false;
	UFUNCTION(Server,Reliable)
	void ServerSetGhostMode(bool bNewGhostMode);




	UPROPERTY()
	int32 MaxRounds = 5;
private:
	float TopScore = 0.f;

	FTimerHandle EndRoundTimerHandle;

	class ABlasterGameMode* BlasterGameMode;
};
