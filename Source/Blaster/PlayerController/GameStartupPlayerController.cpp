// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStartupPlayerController.h"
#include "Blaster/PlayerState/BlasterPlayerState.h" 
#include "Blaster/HUD/LobbyHUD.h"
#include "Blaster/HUD/BlasterHUD.h"
#include "Blaster/HUD/SelectTeam.h"
#include "Components/TextBlock.h"
#include "Blaster/GameMode/LobbyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


void AGameStartupPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGameStartupPlayerController, GameReady);
}

void AGameStartupPlayerController::ServerRequestServerTime_Implementation(float TimeOfClientRequest)
{
	float ServerTimeOfReceipt = GetWorld()->GetTimeSeconds();
	ClientReportServerTime(TimeOfClientRequest, ServerTimeOfReceipt);
}

void AGameStartupPlayerController::ClientReportServerTime_Implementation(float TimeOfClientRequest, float TimeServerRecievedClientRequest)
{
	float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfClientRequest;
	SingleTripTime = (0.5f * RoundTripTime);
	float CurrentServerTime = TimeServerRecievedClientRequest + SingleTripTime;
	ClientServerDelta = CurrentServerTime - GetWorld()->GetTimeSeconds();
}



void AGameStartupPlayerController::Tick(float Deltatime)
{
	Super::Tick(Deltatime);
	ABlasterPlayerState* BPlayerState = GetPlayerState<ABlasterPlayerState>();

	if (BPlayerState)
	{
		//EStooooooooooooo
		FString Data = BPlayerState->GetName();
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, Deltatime, FColor::Orange, Data);
	}

	CheckTimeSync(Deltatime);
	ServerRequestServerGameState();
	PollInit();
	SetHUDMessage();
}

void AGameStartupPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (BlasterHUD) return;
	BlasterHUD = Cast<ABlasterHUD>(GetHUD());
	if (BlasterHUD)
	{
		BlasterHUD->AddLobbyHUD();
	}
}

void AGameStartupPlayerController::HandleTabWidget()
{
	
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	if (BlasterHUD)
	{
		if (BlasterHUD->SelectTeam == nullptr)  BlasterHUD->AddSelectTeam();
		if (BlasterHUD->SelectTeam)
		{
			if (BlasterHUD->SelectTeam->IsVisible())
			{
				BlasterHUD->SelectTeam->SetVisibility(ESlateVisibility::Hidden);
				FInputModeGameOnly InputMode;
				SetInputMode(InputMode);
				bShowMouseCursor = false;
			}
			else 
			{
				BlasterHUD->SelectTeam->SetVisibility(ESlateVisibility::Visible);
				FInputModeGameAndUI  InputMode;
				InputMode.SetWidgetToFocus(BlasterHUD->SelectTeam->TakeWidget());
				InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				SetInputMode(InputMode);
				bShowMouseCursor = true;
			}
		}
	}
}

void AGameStartupPlayerController::JoinToGroup(EGroup NewGroup)
{
	ServerRequestJoinToGroup(NewGroup);
}


void AGameStartupPlayerController::ServerRequestJoinToGroup_Implementation(EGroup NewGroup)
{
	ALobbyGameMode* GameMode = Cast<ALobbyGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		GameMode->AssingPlayerToGroup(this,NewGroup);
	}
}

void AGameStartupPlayerController::UpdateTeamScoreTab(const TArray<FScoreSlotInfo>& GroupA, const TArray<FScoreSlotInfo>& GroupB)
{
	ClientUpdateScoreTab(GroupA, GroupB);
}

void AGameStartupPlayerController::ClientRemoveCurrentHUD_Implementation()
{
	if (BlasterHUD)
	{
		if (BlasterHUD->SelectTeam) BlasterHUD->SelectTeam->RemoveFromParent();
		BlasterHUD->Destroy();
	}
	FInputModeGameOnly IMGameOnly;
	SetInputMode(IMGameOnly);
	bShowMouseCursor = false;

}

void AGameStartupPlayerController::ClientUpdateScoreTab_Implementation(const TArray<FScoreSlotInfo>& GroupA, const TArray<FScoreSlotInfo>& GroupB)
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	if (BlasterHUD == nullptr) return;
	if (BlasterHUD->SelectTeam == nullptr)  BlasterHUD->AddSelectTeam();
	if (BlasterHUD->SelectTeam == nullptr) return;
	BlasterHUD->SelectTeam->UpdateTeamWidget(GroupA, GroupB);
}

void AGameStartupPlayerController::CheckTimeSync(float DeltaTime)
{
	TimeSyncRunningTime += DeltaTime;
	if (IsLocalController() && TimeSyncRunningTime > TimeSyncFrequency)
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
		TimeSyncRunningTime = 0.f;
	}
}

void AGameStartupPlayerController::ServerRequestServerTimeRemaining_Implementation()
{
	ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(UGameplayStatics::GetGameMode(this));
	if (LobbyGameMode == nullptr) return;
	ClientReportServerTimeRemaining(LobbyGameMode->GetTRTT());
}



void AGameStartupPlayerController::ClientReportServerTimeRemaining_Implementation(float ServerTimeRemaining)
{
	TimeRemaining = ServerTimeRemaining;
}

void AGameStartupPlayerController::ServerRequestServerGameState_Implementation()
{
	ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(UGameplayStatics::GetGameMode(this));
	if (LobbyGameMode == nullptr) return;
	if (LobbyGameMode->SessionState == ESessionState::ReadyToPlay) ClientReportServerGameState(true);
	else return ClientReportServerGameState(false);
}

void AGameStartupPlayerController::ClientReportServerGameState_Implementation(bool IsGameReady)
{
	GameReady = IsGameReady;
}
void AGameStartupPlayerController::PollInit()
{
	if (BlasterHUD == nullptr)
	{
		BlasterHUD = Cast<ABlasterHUD>(GetHUD());
		if (BlasterHUD)
		{
			if (BlasterHUD->LobbyHUD == nullptr)
			{
				BlasterHUD->AddLobbyHUD();
			}
		}
	}
}
void AGameStartupPlayerController::SetHUDMessage()
{
	if (BlasterHUD == nullptr || BlasterHUD->LobbyHUD == nullptr) return;


	if (GameReady)
	{
		ServerRequestServerTimeRemaining();
		FString HUDMessage = FString::Printf(TEXT("La partida inicia en %d segundos"), (int32)TimeRemaining);
		BlasterHUD->LobbyHUD->MSG_Text->SetText(FText::FromString(HUDMessage));

	}
	else
	{
		FString HUDMessage = FString::Printf(TEXT("Esperando por jugadores..."));

		BlasterHUD->LobbyHUD->MSG_Text->SetText(FText::FromString(HUDMessage));

	}

}

void AGameStartupPlayerController::OnRep_GameReady()
{
	SetHUDMessage();
}

float AGameStartupPlayerController::GetServerTime()
{
	if (HasAuthority()) return GetWorld()->GetTimeSeconds();
	else return GetWorld()->GetTimeSeconds() + ClientServerDelta;
}
