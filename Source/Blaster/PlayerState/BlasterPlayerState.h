// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Blaster/BlasterTypes/Team.h"
#include "BlasterPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterPlayerState : public APlayerState
{
	GENERATED_BODY()
	//El player state puede almacenar todo tipo de valores aun cuando el jugador sea destruido
	// valores como  el score, las muertes y demas valores, queda activo en el juego
public:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	/**
	* Replication notifies
	*/
	virtual void OnRep_Score() override;
	UFUNCTION()
	virtual void OnRep_Defeats();

	void AddToScore(float ScoreAmount);
	void AddToDefeats(int32 DefeatsAmount);
	bool IsGhostGameMode();
private:
	UPROPERTY()
	class ABlasterCharacter* Character = nullptr;
	UPROPERTY()
	class ABlasterPlayerController* Controller = nullptr;
	UPROPERTY(ReplicatedUsing = OnRep_Defeats)
	int32 Defeats;

	UPROPERTY(ReplicatedUsing = OnRep_Team,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ETeam Team = ETeam::ET_NoTeam;

	UFUNCTION()
	void OnRep_Team();

	UPROPERTY(ReplicatedUsing = OnRep_Ghost, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bGhost = false;

	UFUNCTION()
	void OnRep_Ghost();

public:
	FORCEINLINE ETeam GetTeam() const { return Team; }
	void SetTeam(ETeam TeamToSet);
	void SetGhost(bool NewGhost);
	FORCEINLINE bool GetGhost() { return bGhost; }
	FORCEINLINE ABlasterCharacter* GetCharacter() { return Character; }
	FORCEINLINE void SetCharacter(ABlasterCharacter* InCharacter) { Character = InCharacter; }
	FORCEINLINE float GetDefeats() { return Defeats; }

};
