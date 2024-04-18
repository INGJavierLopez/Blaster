// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BlasterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetHUDHealth(float Health, float MaxHealth);
	void SetHUDShield(float Shield, float MaxShield);
	void SetHUDScore(float Score);
	void SetHUDDefeats(int32 Defeats);
	void SetHUDWeaponAmmo(int32 Ammo);
	void SetHUDCarriedAmmo(int32 Ammo);
	void SetHUDMatchCountdown(float CountdownTime);
	void SetHUDAnnouncementCountdown(float CountdownTime);
	void SetHUDGrenades(int32 Grenades);
	virtual void OnPossess(APawn* InPawn) override;

	virtual void Tick(float Deltatime) override;

	virtual float GetServerTime(); //Sybced with server world clock
	virtual void ReceivedPlayer() override; //Sync with server clock as soon as possible

	void OnMatchStateSet(FName State);

	void HandleMatchHasStarted();
	void HandleCooldown();

	float SingleTripTime = 0;

protected:
	virtual void BeginPlay() override;
	void SetHUDTime();
	void PollInit();
	/**
	*  Sync time between client and server
	*/ 
	//RPC RQUEST the current server time, passing in the clients time when the request was sent
	UFUNCTION(Server,Reliable)
	void ServerRequestServerTime(float TimeOfClientRequest);
	// RPC Reports the current server time to the client in response to ServerRequestTime
	UFUNCTION(Client, Reliable)
	void ClientReportServerTime(float TimeOfClientRequest, float TimeServerRecievedClientRequest);

	float ClientServerDelta = 0.f; // difference between client and server time

	UPROPERTY(EditAnywhere, Category = Time)
	float TimeSyncFrequency = 5.f;

	float TimeSyncRunningTime = 0.f;

	void CheckTimeSync(float DeltaTime);

	//RPCs
	UFUNCTION(Server,Reliable)
	void ServerCheckMatchState();

	UFUNCTION(Client, Reliable)
	void ClientJoinMidgame(FName StateOfMatch, float Warmup, float Match,float Cooldown, float StartingTime);

	void HighPinWarning();
	void StopHighPinWarning();
	void CheckPing(float DeltaTime);
private:
	UPROPERTY()
	class ABlasterHUD* BlasterHUD;
	UPROPERTY()
	class ABlasterGameMode* BlasterGameMode;
	float LevelStartingTime = 0.f;
	float MatchTime = 0.f;
	float WarmupTime = 0.f;
	float CooldownTime = 0.f;
	uint32 CountdownInt = 0;

	UPROPERTY(ReplicatedUsing = OnRep_MatchState)
	FName MatchState;

	//rep nitifier
	UFUNCTION()
	void OnRep_MatchState();

	UPROPERTY()
	class UCharacterOverlay* CharacterOverlay;	

	bool bInitializeHealth = false;
	float HUDHealth;

	bool bInitializeScore = false;
	float HUDMaxHealth;

	bool bInitializeDefeats = false;
	int32 HUDDefeats;

	bool bInitializeGrenades = false;
	int32 HUDGrenades;

	bool bInitializeShield = false;
	float HUDShield;
	
	float HUDScore;
	
	float HUDMaxShield;

	float HUDCarriedAmmo;
	bool bInitializeCarriedAmmo = false;
	float HUDWeaponAmmo;
	bool bInitializeWeaponAmmo;
	
	float HighPingRunningTime = 0.f;

	UPROPERTY(EditAnywhere)
	float HighPingDuration = 5.f;

	UPROPERTY(EditAnywhere)
	float PingAnimationRunningTime = 0.f;

	UPROPERTY(EditAnywhere)
	float CheckPingFrequency = 20.f;

	UPROPERTY(EditAnywhere)
	float HighPingThreshold = 50.f;
};
