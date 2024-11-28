// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameStartupPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API AGameStartupPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> LoadScreenClass;

	virtual void Tick(float Deltatime) override;
	virtual void BeginPlay() override;

	
protected:
	/**
	*  Sync time between client and server
	*/
	//RPC RQUEST the current server time, passing in the clients time when the request was sent
	UFUNCTION(Server, Reliable)
	void ServerRequestServerTime(float TimeOfClientRequest);
	// RPC Reports the current server time to the client in response to ServerRequestTime
	UFUNCTION(Client, Reliable)
	void ClientReportServerTime(float TimeOfClientRequest, float TimeServerRecievedClientRequest);

	float ClientServerDelta = 0.f; // difference between client and server time
	UPROPERTY(EditAnywhere, Category = Time)
	float TimeSyncFrequency = 5.f;

	float TimeSyncRunningTime = 0.f;
	void CheckTimeSync(float DeltaTime);

	//RPC RQUEST the current server time remaining to travel
	UFUNCTION(Server, Reliable)
	void ServerRequestServerTimeRemaining();
	// RPC Reports the current server time remaining to the client in response to ServerRequestServerTimeRemaining
	UFUNCTION(Client, Reliable)
	void ClientReportServerTimeRemaining(float ServerTimeRemaining);

	//RPC Pide al Servidor 
	UFUNCTION(Server, Reliable)
	void ServerRequestServerGameState();
	// RPC Reports the current server time to the client in response to ServerRequestTime
	UFUNCTION(Client, Reliable)
	void ClientReportServerGameState(bool IsGameReady);

private:
	void PollInit();
	void SetHUDMessage();

	UFUNCTION()
	void OnRep_GameReady();

	UPROPERTY(ReplicatedUsing = OnRep_GameReady)
	bool GameReady;
	/*
	*  HUD
	*/
	
	UPROPERTY()
	class ABlasterHUD* BlasterHUD;

	float TimeRemaining;
public:
	virtual float GetServerTime(); //Synced with server world clock
	float SingleTripTime = 0;

};
