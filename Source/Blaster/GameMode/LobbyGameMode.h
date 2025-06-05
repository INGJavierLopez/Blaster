// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Blaster/BlasterTypes/ScoreTabStructures.h"
#include "Blaster/BlasterTypes/Group.h"
#include "LobbyGameMode.generated.h"

UENUM(BlueprintType)
enum class ESessionState : uint8
{
	WaitingForPlayers UMETA(DisplayName = "Waiting For Players"),
	ReadyToPlay UMETA(DisplayName = "Ready To Play")
};

/**
 * 
 */
UCLASS()
class BLASTER_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	UFUNCTION(BlueprintImplementableEvent)
	void Prueba(int32 dato, float DeltaSeconds);

	virtual void BeginPlay();

	virtual void Tick(float DeltaSeconds) override;
	void Debug(float DeltaSeconds);
	UPROPERTY(BlueprintReadWrite)
	ESessionState SessionState = ESessionState::WaitingForPlayers;
	//Borrar
	UFUNCTION(BlueprintCallable)
	void WaitGameMode();

	bool flag = false;

	void AssingPlayerToGroup(APlayerController* PlayerController,EGroup NewGroup);
protected:
	void StartGame();

private:

	class UGameInstance* GameInstance;

	UPROPERTY(EditDefaultsOnly)
	float TimeToTravel = 5.f;

	float TimeStartCount = 0.f;
	float TimeRemainingToTravel = 0.f;
	bool ReadyToTravel = false;
	bool StartCounter = false;

	class AGameStartupPlayerController* GameStartupPC;


	TArray< APlayerController*> PendingPlayerControllers;
	void UpdateTeamsTable();

	void CheckLocalPlayers(float DeltaTime);
	AGameStartupPlayerController* GetPlayerControllerByNetID(UWorld* World, const FString& NetID);
public:
	FORCEINLINE float GetTRTT() { return TimeRemainingToTravel; }
};
