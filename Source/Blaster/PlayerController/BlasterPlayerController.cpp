// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterPlayerController.h"
#include "Blaster/HUD/BlasterHUD.h"
#include "Blaster/HUD/CharacterOverlay.h"
#include "Blaster/HUD/NewRound.h"
#include "Blaster/HUD/EndRound.h"
#include "Blaster/HUD/EndGame.h"
#include "Blaster/HUD/LobbyHUD.h"
#include "Blaster/HUD/TeamScoreTab.h"
#include "Blaster/HUD/ScoreTab.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Blaster/GameMode/BlasterGameMode.h"
#include "Blaster/PlayerState/BlasterPlayerState.h"
#include "Blaster/HUD/Announcement.h"
#include "Kismet/GameplayStatics.h"
#include "Blaster/BlasterComponents/CombatComponent.h"
#include "Blaster/GameState/BlasterGameState.h"
#include "Components/Image.h"
#include "Blaster/HUD/ReturnToMainMenu.h"
#include "Blaster/BlasterTypes/Announcement.h"
#include "Blaster/GameInstance/BlasterGameInstance.h"






void ABlasterPlayerController::InitTeamScores()
{
	UE_LOG(LogTemp, Warning, TEXT("LLAMADO INITTEAMSCORES"));

	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->RedTeamScore &&
		BlasterHUD->CharacterOverlay->BlueTeamScore &&
		BlasterHUD->CharacterOverlay->RedTeamRounds &&
		BlasterHUD->CharacterOverlay->BlueTeamRounds &&
		BlasterHUD->CharacterOverlay->TeamsScoreImage &&
		BlasterHUD->CharacterOverlay->BlueTeamScoreImage &&
		BlasterHUD->CharacterOverlay->RedTeamScoreImage;

	if (bHUDValid)
	{
		FString Zero("0");
		ABlasterGameState* BlasterGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));

		BlasterHUD->CharacterOverlay->RedTeamScore->SetText(FText::FromString(Zero));
		BlasterHUD->CharacterOverlay->BlueTeamScore->SetText(FText::FromString(Zero));
		if (BlasterGameState)
		{
			FString RTRounds = FString::Printf(TEXT("%d"), (int32)BlasterGameState->RedTeamRoundScore);
			FString BTRounds = FString::Printf(TEXT("%d"), (int32)BlasterGameState->BlueTeamRoundScore);

			BlasterHUD->CharacterOverlay->RedTeamRounds->SetText(FText::FromString(RTRounds));
			BlasterHUD->CharacterOverlay->BlueTeamRounds->SetText(FText::FromString(BTRounds));
		}
		else
		{
			BlasterHUD->CharacterOverlay->RedTeamRounds->SetText(FText::FromString(Zero));
			BlasterHUD->CharacterOverlay->BlueTeamRounds->SetText(FText::FromString(Zero));
		}
		BlasterHUD->CharacterOverlay->TeamsScoreImage->SetVisibility(ESlateVisibility::Visible);
		BlasterHUD->CharacterOverlay->BlueTeamScoreImage->SetVisibility(ESlateVisibility::Visible);
		BlasterHUD->CharacterOverlay->RedTeamScoreImage->SetVisibility(ESlateVisibility::Visible);
	}
}

void ABlasterPlayerController::HideTeamScores()
{
	UE_LOG(LogTemp, Warning, TEXT("LLAMADO HIDETEAMSCORES"));
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->RedTeamScore &&
		BlasterHUD->CharacterOverlay->BlueTeamScore &&
		BlasterHUD->CharacterOverlay->RedTeamRounds &&
		BlasterHUD->CharacterOverlay->BlueTeamRounds &&
		BlasterHUD->CharacterOverlay->TeamsScoreImage &&
		BlasterHUD->CharacterOverlay->BlueTeamScoreImage &&

		BlasterHUD->CharacterOverlay->RedTeamScoreImage;

	if (bHUDValid)
	{
		BlasterHUD->CharacterOverlay->RedTeamScore->SetText(FText());
		BlasterHUD->CharacterOverlay->BlueTeamScore->SetText(FText());
		BlasterHUD->CharacterOverlay->RedTeamRounds->SetText(FText());
		BlasterHUD->CharacterOverlay->BlueTeamRounds->SetText(FText());
		BlasterHUD->CharacterOverlay->TeamsScoreImage->SetVisibility(ESlateVisibility::Hidden);
		BlasterHUD->CharacterOverlay->BlueTeamScoreImage->SetVisibility(ESlateVisibility::Hidden);
		BlasterHUD->CharacterOverlay->RedTeamScoreImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ABlasterPlayerController::SetHUDRedTeamScore(int32 RedScore)
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->RedTeamScore;
	if (bHUDValid)
	{
		FString ScoreText = FString::Printf(TEXT("%d"), RedScore);
		BlasterHUD->CharacterOverlay->RedTeamScore->SetText(FText::FromString(ScoreText));
	}
}

void ABlasterPlayerController::SetHUDBlueTeamScore(int32 BlueScore)
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->BlueTeamScore;
	if (bHUDValid)
	{
		FString ScoreText = FString::Printf(TEXT("%d"), BlueScore);
		BlasterHUD->CharacterOverlay->BlueTeamScore->SetText(FText::FromString(ScoreText));
	}
}

void ABlasterPlayerController::SetHUDRedTeamRounds(int32 RedRounds)
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->RedTeamRounds;
	if (bHUDValid)
	{
		FString ScoreText = FString::Printf(TEXT("%d"), RedRounds);
		BlasterHUD->CharacterOverlay->RedTeamRounds->SetText(FText::FromString(ScoreText));
	}
}

void ABlasterPlayerController::SetHUDBlueTeamRounds(int32 BlueRounds)
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->BlueTeamRounds;
	if (bHUDValid)
	{
		FString ScoreText = FString::Printf(TEXT("%d"), BlueRounds);
		BlasterHUD->CharacterOverlay->BlueTeamRounds->SetText(FText::FromString(ScoreText));
	}
}

void ABlasterPlayerController::BroadcastElim(APlayerState* Attacker, APlayerState* Victim)
{
	ClientElimAnnouncement(Attacker, Victim);
}

void ABlasterPlayerController::ClientElimAnnouncement_Implementation(APlayerState* Attacker, APlayerState* Victim)
{
	APlayerState* Self = GetPlayerState<APlayerState>();
	if (Attacker && Victim)
	{
		BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
		if (BlasterHUD)
		{
			FString ElimAnnouncementText;
			if (Attacker == Self && Victim != Self)
			{
				ElimAnnouncementText = FString::Printf(TEXT("Eliminaste a %s"),*Victim->GetPlayerName());
				BlasterHUD->AddElimAnnouncement(ElimAnnouncementText);
				return;
			}
			if (Victim == Self && Attacker != Self)
			{
				ElimAnnouncementText = FString::Printf(TEXT("%s te ha eliminado"), *Attacker->GetPlayerName());
				BlasterHUD->AddElimAnnouncement(ElimAnnouncementText);
				return;
			}
			if (Attacker == Victim && Attacker == Self)
			{
				ElimAnnouncementText = FString::Printf(TEXT("Te eliminaste a ti mismo"));
				BlasterHUD->AddElimAnnouncement(ElimAnnouncementText);
				return;
			}
			if (Attacker == Victim && Attacker != Self)
			{
				ElimAnnouncementText = FString::Printf(TEXT("%s se elimino a si mismo"), *Victim->GetPlayerName());
				BlasterHUD->AddElimAnnouncement(ElimAnnouncementText);
				return;
			}
			ElimAnnouncementText = FString::Printf(TEXT("%s elimino a %s"),*Attacker->GetPlayerName(),*Victim->GetPlayerName());
			BlasterHUD->AddElimAnnouncement(ElimAnnouncementText);
		}
	}
}

void ABlasterPlayerController::SetPlayerGameplayMovement(bool Enable)
{
	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(GetPawn());

	if (BlasterCharacter)
	{
		if (Enable)
		{
			BlasterCharacter->EnableMovement();
		}
		else
		{
			BlasterCharacter->DisableMovement();
		}
	}
}

void ABlasterPlayerController::HandleRoundScore(float Red, float Blue, int32 CurrentRound, EEndRoundType EndMatchType)
{
	//Crea el widget de ronda finalizada
	//actualiza el score actual de las rondas
	//SetHUDTime actualiza el tiempo
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	if (BlasterHUD)
	{
		if (BlasterHUD->EndRound == nullptr) BlasterHUD->AddEndRound();
		if (BlasterHUD->EndRound)
		{
			if (!BlasterHUD->EndRound->IsVisible()) BlasterHUD->EndRound->SetVisibility(ESlateVisibility::Visible);
			bool EndRoundValid = BlasterHUD->EndRound->RedTeamScoreText &&
				BlasterHUD->EndRound->BlueTeamScoreText &&
				BlasterHUD->EndRound->RoundText &&
				BlasterHUD->EndRound->NextRoundText &&
				BlasterHUD->EndRound->WinnerText;
			if (!EndRoundValid) return;

			FString PastRoundText = FString::Printf(TEXT("Ronda %d Finalizada"),CurrentRound -1);
			BlasterHUD->EndRound->RoundText->SetText(FText::FromString(PastRoundText));

			FString Winner;
			if (EndMatchType == EEndRoundType::ERT_RedTeam)
			{
				Winner = FString::Printf(TEXT("Gana el equipo Rojo"), CurrentRound - 1);
			}
			else if (EndMatchType == EEndRoundType::ERT_BlueTeam)
			{
				Winner = FString::Printf(TEXT("Gana el equipo Azul"), CurrentRound - 1);
			}
			else if (EndMatchType == EEndRoundType::ERT_Draw)
			{
				Winner = FString::Printf(TEXT("Empate"), CurrentRound - 1);
			}
			BlasterHUD->EndRound->WinnerText->SetText(FText::FromString(Winner));


			FString RedTeamText = FString::Printf(TEXT("%d"), (int32)Red);
			BlasterHUD->EndRound->RedTeamScoreText->SetText(FText::FromString(RedTeamText));
			//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, RedTeamText);

			FString BlueTeamText = FString::Printf(TEXT("%d"), (int32)Blue);
			BlasterHUD->EndRound->BlueTeamScoreText->SetText(FText::FromString(BlueTeamText));
			//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, BlueTeamText);

			FString NewRoundText = FString::Printf(TEXT("Ronda %d"), CurrentRound);
			BlasterHUD->EndRound->NextRoundText->SetText(FText::FromString(NewRoundText));
			switch (EndMatchType)
			{
				case EEndRoundType::ERT_RedTeam:
					BlasterHUD->EndRound->PlayAnimTeamRoundScores(false);
					break;
				case EEndRoundType::ERT_BlueTeam:
					BlasterHUD->EndRound->PlayAnimTeamRoundScores(true);
					break;
				default:
					break;
			}
		}
		
		if (BlasterHUD->CharacterOverlay == nullptr) BlasterHUD->AddCharacterOverlay();
		if (BlasterHUD->CharacterOverlay == nullptr) return;
		FString NuevaRondaText = FString::Printf(TEXT("Ronda %d"), CurrentRound);
		BlasterHUD->CharacterOverlay->CurrentRoundText->SetText(FText::FromString(NuevaRondaText));
	}
	
}

void ABlasterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	BlasterHUD = Cast<ABlasterHUD>(GetHUD());
	ServerCheckMatchState();

}

void ABlasterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (InputComponent == nullptr) return;

	InputComponent->BindAction("Quit",IE_Pressed,this,&ABlasterPlayerController::ShowReturnToMainMenu);
}


void ABlasterPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABlasterPlayerController, MatchState);
	DOREPLIFETIME(ABlasterPlayerController, bShowTeamScores);
}

void ABlasterPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CheckTimeSync(DeltaTime);
	SetHUDTime();
	PollInit();
	CheckPing(DeltaTime);
	if (PlayerState)
	{
		FString Data = FString::Printf(TEXT("Ping : %d"), PlayerState->GetCompressedPing() * 4);
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, DELTA, FColor::Orange, Data);
	}
	return;
	UBlasterGameInstance* BlasterGameInstance = Cast<UBlasterGameInstance>(GetGameInstance());
	if (BlasterGameInstance)
	{
		if (GetLocalPlayer() && GEngine)
		{
			FString ID = GetLocalPlayer()->GetPreferredUniqueNetId()->ToString();
			EGroup Grupo = BlasterGameInstance->GetPlayerGroup(ID);
			FString Text;
			switch (Grupo)
			{
			case EGroup::EG_A:
				Text = FString(TEXT("Equipo Azul"));
				GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Orange, Text);
				break;
			case EGroup::EG_B:
				Text = FString(TEXT("Equipo Rojo"));
				GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Orange, Text);
				break;
			case EGroup::EG_NG:
				Text = FString(TEXT("NO TIENE EQUIPO"));
				GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Orange, Text);
				break;
			default:
				break;
			}
		}
	}
	
}

void ABlasterPlayerController::CheckPing(float DeltaTime)
{
	HighPingRunningTime += DeltaTime;
	if (HighPingRunningTime > CheckPingFrequency)
	{
		HighPingRunningTime = 0;
		PlayerState = PlayerState == nullptr ? GetPlayerState<APlayerState>() : PlayerState;
		if (PlayerState)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Ping: %d"), PlayerState->GetPing() * 4);
			if (PlayerState->GetCompressedPing() * 4 > HighPingThreshold) // ping is compressed ; its divided by 4
			{
				HighPinWarning();
				PingAnimationRunningTime = 0.f;
				ServerReportPingStatus(true);
			}
			else
			{
				ServerReportPingStatus(false);
			}
		}
	}
	bool bHighPingAnimationPlaying =
		BlasterHUD && BlasterHUD->CharacterOverlay && BlasterHUD->CharacterOverlay->HighPingAnimation && BlasterHUD->CharacterOverlay->IsAnimationPlaying(BlasterHUD->CharacterOverlay->HighPingAnimation);
	if (bHighPingAnimationPlaying)
	{
		PingAnimationRunningTime += DeltaTime;
		if (PingAnimationRunningTime > HighPingDuration)
		{
			StopHighPinWarning();
		}
	}
}

void ABlasterPlayerController::ShowReturnToMainMenu()
{
	// TODO Show the return to Main Menu Widget
	if (ReturnToMainMenuWidget == nullptr) return;
	if (ReturnToMainMenu == nullptr)
	{
		ReturnToMainMenu = CreateWidget<UReturnToMainMenu>(this, ReturnToMainMenuWidget);
	}
	if (ReturnToMainMenu)
	{
		bReturnToMainMenuOpen = !bReturnToMainMenuOpen;
		if (bReturnToMainMenuOpen)
		{
			ReturnToMainMenu->MenuSetup();
		}
		else
		{
			ReturnToMainMenu->MenuTearDown();
		}
	}
}




//Is the ping too high?
void ABlasterPlayerController::ServerReportPingStatus_Implementation(bool bHighPin)
{
	HighPingDelegate.Broadcast(bHighPin);
}

void ABlasterPlayerController::CheckTimeSync(float DeltaTime)
{
	TimeSyncRunningTime += DeltaTime;
	if (IsLocalController() && TimeSyncRunningTime > TimeSyncFrequency)
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
		TimeSyncRunningTime = 0.f;
	}
}

void ABlasterPlayerController::HighPinWarning()
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->HighPingImage &&
		BlasterHUD->CharacterOverlay->HighPingAnimation;
	if (bHUDValid)
	{
		BlasterHUD->CharacterOverlay->HighPingImage->SetOpacity(1.f);
		BlasterHUD->CharacterOverlay->PlayAnimation(
			BlasterHUD->CharacterOverlay->HighPingAnimation,
			0.f,
			5
		);
	}
}

void ABlasterPlayerController::StopHighPinWarning()
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->HighPingImage &&
		BlasterHUD->CharacterOverlay->HighPingAnimation;
	if (bHUDValid)
	{
		BlasterHUD->CharacterOverlay->HighPingImage->SetOpacity(0.f);
		if (BlasterHUD->CharacterOverlay->IsAnimationPlaying(BlasterHUD->CharacterOverlay->HighPingAnimation))
		{
			BlasterHUD->CharacterOverlay->StopAnimation(BlasterHUD->CharacterOverlay->HighPingAnimation);
		}
	}
}

void ABlasterPlayerController::ServerCheckMatchState_Implementation()
{
	ABlasterGameMode* GameMode = Cast<ABlasterGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		bShowTeamScores = GameMode->bTeamsMatch; // added this while trying to debug the client not having the teams match spawned

		WarmupTime = GameMode->WarmupTime;
		MatchTime = GameMode->MatchTime;
		CooldownTime = GameMode->CooldownTime;
		LevelStartingTime = GameMode->LevelStartingTime;
		MatchState = GameMode->GetMatchState();
		RoundStartTime = GameMode->GetStateStartTime();
		ClientJoinMidgame(MatchState, WarmupTime, MatchTime, CooldownTime, LevelStartingTime, bShowTeamScores, RoundStartTime); // added the bShowTeamScores to the Client joinmidgame so we can pass it to OnMatchState Set
	}
}

void ABlasterPlayerController::ClientJoinMidgame_Implementation(FName StateOfMatch, float Warmup, float Match, float Cooldown, float StartingTime, bool bIsTeamsMatch,float InRoundStartTime)
{
	WarmupTime = Warmup;
	MatchTime = Match;
	CooldownTime = Cooldown;
	LevelStartingTime = StartingTime;
	MatchState = StateOfMatch;
	RoundStartTime = InRoundStartTime;
	OnMatchStateSet(MatchState, bIsTeamsMatch); // added the bIsTeamsMatch here
}

void ABlasterPlayerController::HandleTabWidget(bool show)
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	if (BlasterHUD)
	{
		if (bShowTeamScores)
		{
			if (BlasterHUD->TeamScoreTab == nullptr)  BlasterHUD->AddTeamScoreTab();
			if (BlasterHUD->TeamScoreTab)
			{
				if(show) BlasterHUD->TeamScoreTab->SetVisibility(ESlateVisibility::Visible);
				else BlasterHUD->TeamScoreTab->SetVisibility(ESlateVisibility::Hidden);
			}
		}
		else
		{
			if (BlasterHUD->ScoreTab == nullptr)  BlasterHUD->AddScoreTab();
			if (BlasterHUD->ScoreTab)
			{
				if (show) BlasterHUD->ScoreTab->SetVisibility(ESlateVisibility::Visible);
				else BlasterHUD->ScoreTab->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void ABlasterPlayerController::UpdateTeamScoreTab(const TArray<FScoreSlotInfo>& GroupA, const TArray<FScoreSlotInfo>& GroupB)
{
	
	ClientUpdateScoreTab(GroupA, GroupB);
}



void ABlasterPlayerController::ClientUpdateScoreTab_Implementation(const TArray<FScoreSlotInfo>& GroupA, const TArray<FScoreSlotInfo>& GroupB)
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	if (BlasterHUD == nullptr) return;
	if (BlasterHUD->TeamScoreTab == nullptr)  BlasterHUD->AddTeamScoreTab();
	if (BlasterHUD->TeamScoreTab == nullptr) return;
	BlasterHUD->TeamScoreTab->UpdateTables(GroupA, GroupB);
	//BlasterHUD->TeamScoreTab->UpdateWidgetTeamScoreTab(GroupA, GroupB);
	if (BlasterHUD->EndRound) BlasterHUD->EndRound->SetVisibility(ESlateVisibility::Hidden);
}

void ABlasterPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(InPawn);
	if (BlasterCharacter)
	{
		SetHUDHealth(BlasterCharacter->GetHealth(), BlasterCharacter->GetMaxHealth());
	}
}

void ABlasterPlayerController::SetHUDHealth(float Health, float MaxHealth)
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->HealthBar &&
		BlasterHUD->CharacterOverlay->HealthText;
	if (bHUDValid)
	{
		const float HealthPercent = Health / MaxHealth;
		BlasterHUD->CharacterOverlay->HealthBar->SetPercent(HealthPercent);
		FString HealthText = FString::Printf(TEXT("%d"), FMath::CeilToInt(Health));
		BlasterHUD->CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));
	}
	else
	{
		bInitializeHealth = true;
		HUDHealth = Health;
		HUDMaxHealth = MaxHealth;
	}
}

void ABlasterPlayerController::SetHUDShield(float Shield, float MaxShield)
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->ShieldBar &&
		BlasterHUD->CharacterOverlay->ShieldText;
	if (bHUDValid)
	{
		const float ShieldPercent = Shield / MaxShield;
		BlasterHUD->CharacterOverlay->ShieldBar->SetPercent(ShieldPercent);
		FString ShieldText = FString::Printf(TEXT("%d"), FMath::CeilToInt(Shield));
		BlasterHUD->CharacterOverlay->ShieldText->SetText(FText::FromString(ShieldText));
	}
	else
	{
		bInitializeShield = true;
		HUDShield = Shield;
		HUDMaxShield = MaxShield;
	}
}

void ABlasterPlayerController::SetHUDScore(float Score)
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->ScoreAmount;

	if (bHUDValid)
	{
		FString ScoreText = FString::Printf(TEXT("%d"), FMath::FloorToInt(Score));
		BlasterHUD->CharacterOverlay->ScoreAmount->SetText(FText::FromString(ScoreText));
	}
	else
	{
		bInitializeScore = true;
		HUDScore = Score;
	}
}

void ABlasterPlayerController::SetHUDDefeats(int32 Defeats)
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->DefeatsAmount;
	if (bHUDValid)
	{
		FString DefeatsText = FString::Printf(TEXT("%d"), Defeats);
		BlasterHUD->CharacterOverlay->DefeatsAmount->SetText(FText::FromString(DefeatsText));
	}
	else
	{
		bInitializeDefeats = true;
		HUDDefeats = Defeats;
	}
}

void ABlasterPlayerController::SetHUDWeaponAmmo(int32 Ammo)
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->WeaponAmmoAmount;
	if (bHUDValid)
	{
		FString AmmoText = FString::Printf(TEXT("%d"), Ammo);
		BlasterHUD->CharacterOverlay->WeaponAmmoAmount->SetText(FText::FromString(AmmoText));
	}
	else
	{
		bInitializeWeaponAmmo = true;
		HUDWeaponAmmo = Ammo;
	}
}

void ABlasterPlayerController::SetHUDCarriedAmmo(int32 Ammo)
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->CarriedAmmoAmount;
	if (bHUDValid)
	{
		FString AmmoText = FString::Printf(TEXT("%d"), Ammo);
		BlasterHUD->CharacterOverlay->CarriedAmmoAmount->SetText(FText::FromString(AmmoText));
	}
	else
	{
		bInitializeCarriedAmmo = true;
		HUDCarriedAmmo = Ammo;
	}
}

void ABlasterPlayerController::SetHUDMatchCountdown(float CountdownTime)
{
	UE_LOG(LogTemp, Warning, TEXT("CDWN Time: %f"), CountdownTime);
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->MatchCountdownText;
	if (bHUDValid)
	{
		if (CountdownTime < 0.f)
		{
			BlasterHUD->CharacterOverlay->MatchCountdownText->SetText(FText());
			return;
		}

		int32 Minutes = FMath::FloorToInt(CountdownTime / 60.f);
		int32 Seconds = CountdownTime - Minutes * 60;

		FString CountdownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		BlasterHUD->CharacterOverlay->MatchCountdownText->SetText(FText::FromString(CountdownText));
	}
}

void ABlasterPlayerController::SetHUDAnnouncementCountdown(float CountdownTime)
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->Announcement &&
		BlasterHUD->Announcement->WarmupTime;
	if (bHUDValid)
	{
		if (CountdownTime < 0.f)
		{
			BlasterHUD->Announcement->WarmupTime->SetText(FText());
			return;
		}

		int32 Minutes = FMath::FloorToInt(CountdownTime / 60.f);
		int32 Seconds = CountdownTime - Minutes * 60;

		FString CountdownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		BlasterHUD->Announcement->WarmupTime->SetText(FText::FromString(CountdownText));
		
	}
}

void ABlasterPlayerController::SetHUDEndRound(float TimeLeft)
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->EndRound &&
		BlasterHUD->EndRound->NextRoundTimeText;
	//Establecer el tiempo restante el el Widget de End Round
	if (bHUDValid)
	{
		int32 Minutes = FMath::FloorToInt(TimeLeft / 60.f);
		int32 Seconds = TimeLeft - Minutes * 60;

		FString CountdownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		BlasterHUD->EndRound->NextRoundTimeText->SetText(FText::FromString(CountdownText));
	}
}


void ABlasterPlayerController::SetHUDGrenades(int32 Grenades)
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD &&
		BlasterHUD->CharacterOverlay &&
		BlasterHUD->CharacterOverlay->GrenadesText;
	if (bHUDValid)
	{
		FString GrenadesText = FString::Printf(TEXT("%d"), Grenades);
		BlasterHUD->CharacterOverlay->GrenadesText->SetText(FText::FromString(GrenadesText));
	}
	else
	{
		bInitializeGrenades = true;
		HUDGrenades = Grenades;
	}
}

void ABlasterPlayerController::SetHUDTime()
{
	float TimeLeft = 0.f;
	if (MatchState == MatchState::WaitingToStart) TimeLeft = WarmupTime - GetServerTime() + LevelStartingTime;
	else if (MatchState == MatchState::MatchInProgress) TimeLeft = MatchTime - (GetServerTime() + LevelStartingTime - RoundStartTime);
	else if (MatchState == MatchState::Cooldown) TimeLeft = CooldownTime - (GetServerTime() - EndRoundTime);

	uint32 SecondsLeft = FMath::CeilToInt(TimeLeft);
	if (CountdownInt != SecondsLeft)
	{
		if (MatchState == MatchState::WaitingToStart)
		{
			SetHUDAnnouncementCountdown(TimeLeft);
		}
		if (MatchState == MatchState::MatchInProgress)
		{
			SetHUDMatchCountdown(TimeLeft);
		}
		if (MatchState == MatchState::Cooldown)
		{
			SetHUDEndRound(TimeLeft);
		}
	}

	CountdownInt = SecondsLeft;
}



void ABlasterPlayerController::PollInit()
{
	if (CharacterOverlay == nullptr)
	{
		if (BlasterHUD && BlasterHUD->CharacterOverlay)
		{
			CharacterOverlay = BlasterHUD->CharacterOverlay;
			if (CharacterOverlay)
			{
				if (bInitializeHealth) SetHUDHealth(HUDHealth, HUDMaxHealth);
				if (bInitializeShield)SetHUDShield(HUDShield, HUDMaxShield);
				if (bInitializeScore)SetHUDScore(HUDScore);
				if (bInitializeDefeats)SetHUDDefeats(HUDDefeats);
				if (bInitializeCarriedAmmo)SetHUDCarriedAmmo(HUDCarriedAmmo);
				if (bInitializeWeaponAmmo) SetHUDWeaponAmmo(HUDWeaponAmmo);
				ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(GetPawn());
				if (BlasterCharacter && BlasterCharacter->GetCombat())
				{
					if (bInitializeGrenades) SetHUDGrenades(BlasterCharacter->GetCombat()->GetGrenades());

				}
			}
		}
	}
}

void ABlasterPlayerController::ServerRequestServerTime_Implementation(float TimeOfClientRequest)
{
	float ServerTimeOfReceipt = GetWorld()->GetTimeSeconds();
	ClientReportServerTime(TimeOfClientRequest, ServerTimeOfReceipt);
}

void ABlasterPlayerController::ClientReportServerTime_Implementation(float TimeOfClientRequest, float TimeServerReceivedClientRequest)
{
	float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfClientRequest;
	SingleTripTime = (0.5f * RoundTripTime);
	float CurrentServerTime = TimeServerReceivedClientRequest + SingleTripTime;
	ClientServerDelta = CurrentServerTime - GetWorld()->GetTimeSeconds();
}

float ABlasterPlayerController::GetServerTime()
{
	if (HasAuthority()) return GetWorld()->GetTimeSeconds();
	else return GetWorld()->GetTimeSeconds() + ClientServerDelta;
}

void ABlasterPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();
	if (IsLocalController())
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
	}
}

void ABlasterPlayerController::OnMatchStateSet(FName State,bool bTeamsMatch)
{
	MatchState = State; 
	if (MatchState == MatchState::WaitingToStart)
	{
		HandleWaitingToStart();
	}
	else if (MatchState == MatchState::NewRound)
	{
		HandleNewRound();
	}
	else if (MatchState == MatchState::MatchInProgress)
	{
		HandleMatchHasStarted(bTeamsMatch);
	}
	else if (MatchState == MatchState::Cooldown)
	{
		HandleCooldown();
	}
	else if (MatchState == MatchState::EndGame)
	{
		HandleEndGame();
	}
}

void ABlasterPlayerController::OnRep_MatchState()
{
	if (MatchState == MatchState::WaitingToStart)
	{
		HandleWaitingToStart();
	}
	else if (MatchState == MatchState::NewRound)
	{
		HandleNewRound();
	}
	else if (MatchState == MatchState::MatchInProgress)
	{
		HandleMatchHasStarted(bShowTeamScores);
	}
	else if (MatchState == MatchState::Cooldown)
	{
		HandleCooldown();
	}
	else if (MatchState == MatchState::EndGame)
	{
		HandleEndGame();
	}
}

bool ABlasterPlayerController::IsMatchInProgress()
{
	if (MatchState == MatchState::MatchInProgress) return true;

	return false;
}

void ABlasterPlayerController::OnRep_ShowTeamScores()
{
	if (bShowTeamScores)
	{
		InitTeamScores();
	}
	else
	{
		HideTeamScores();
	}
}

void ABlasterPlayerController::HandleWaitingToStart()
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	if (BlasterHUD && BlasterHUD->AnnouncementClass)
	{
		if (BlasterHUD->Announcement == nullptr) BlasterHUD->AddAnnouncement();

	}
}



void ABlasterPlayerController::HandleNewRound()
{

	SetPlayerGameplayMovement(false);
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	if (BlasterHUD == nullptr) return;
	
	if (BlasterHUD->NewRound == nullptr) BlasterHUD->AddNewRound();
	if (BlasterHUD->NewRound == nullptr) return;
	if (!BlasterHUD->NewRound->IsVisible()) BlasterHUD->NewRound->SetVisibility(ESlateVisibility::Visible);
	ABlasterGameState* BlasterGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));
	if (BlasterGameState == nullptr) return;
	FString Data = FString::Printf(TEXT("Ronda %d"), BlasterGameState->CurrentRound);
	if (BlasterHUD->NewRound->CurrentRoundText) BlasterHUD->NewRound->CurrentRoundText->SetText(FText::FromString(Data));

	InitTeamScores();

	if (BlasterHUD->Announcement)
	{
		BlasterHUD->Announcement->SetVisibility(ESlateVisibility::Hidden); //Ocultar Announcement
	}
	if (BlasterHUD->CharacterOverlay)
	{
		BlasterHUD->CharacterOverlay->SetVisibility(ESlateVisibility::Hidden); //Ocultar Overlay
	}
	if (BlasterHUD->EndRound)
	{
		BlasterHUD->EndRound->SetVisibility(ESlateVisibility::Hidden); //ocultar End Round
	}
	//desactivar movimiento
	SetPlayerGameplayMovement(false);
}

void ABlasterPlayerController::HandleMatchHasStarted(bool bTeamsMatch)
{

	if (HasAuthority()) bShowTeamScores = bTeamsMatch;
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	if (BlasterHUD)
	{
		if (BlasterHUD->LobbyHUD) BlasterHUD->LobbyHUD->RemoveFromParent();
		if (BlasterHUD->CharacterOverlay == nullptr) BlasterHUD->AddCharacterOverlay();
		if (!BlasterHUD->CharacterOverlay->IsVisible()) BlasterHUD->CharacterOverlay->SetVisibility(ESlateVisibility::Visible);
		if (BlasterHUD->Announcement)
		{
			BlasterHUD->Announcement->SetVisibility(ESlateVisibility::Hidden);
		}
		if (BlasterHUD->NewRound)
		{
			BlasterHUD->NewRound->SetVisibility(ESlateVisibility::Hidden);
		}
		if (BlasterHUD->EndRound)
		{
			BlasterHUD->EndRound->SetVisibility(ESlateVisibility::Hidden);
		}
		if (bTeamsMatch)
		{
			InitTeamScores();
		}
		else
		{
			HideTeamScores();
		}
		RoundStartTime = GetServerTime();
	}
	SetPlayerGameplayMovement(true);

}


void ABlasterPlayerController::HandleCooldown()
{
	/* Desactivacion de movimiento*/
	SetPlayerGameplayMovement(false);
	EndRoundTime = GetServerTime();
	return;

	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;

	if (BlasterHUD && BlasterHUD->EndRoundClass)
	{
		if (BlasterHUD->EndRound == nullptr) BlasterHUD->AddEndRound();
		ABlasterGameState* BlasterGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));
		return;
		BlasterHUD->CharacterOverlay->SetVisibility(ESlateVisibility::Hidden);
		bool bHUDValid = BlasterHUD->Announcement &&
			BlasterHUD->Announcement->AnnouncementText &&
			BlasterHUD->Announcement->InformationText;

		if (bHUDValid)
		{		
			/*IMPORANTE */
			BlasterHUD->Announcement->SetVisibility(ESlateVisibility::Visible);
			FString AnnouncementText = Announcement::NewMatchStartsIn;
			BlasterHUD->Announcement->AnnouncementText->SetText(FText::FromString(AnnouncementText));

			//ABlasterGameState* BlasterGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));
			ABlasterPlayerState* BlasterPlayerState = GetPlayerState<ABlasterPlayerState>();
			if (BlasterGameState && BlasterPlayerState)
			{
				TArray<ABlasterPlayerState*> TopPlayers = BlasterGameState->TopScoringPlayers;
				FString InfoTextString = bShowTeamScores ? GetTeamsInfoText(BlasterGameState) : GetInfoText(TopPlayers);
				UE_LOG(LogTemp, Warning, TEXT("%s"),*InfoTextString);
				BlasterHUD->Announcement->InformationText->SetText(FText::FromString(InfoTextString));
			}
		}
	}
	
}

void ABlasterPlayerController::HandleEndGame()
{
}

void ABlasterPlayerController::ShowGameWinner(ETeam WinnerTeam)
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	if (BlasterHUD)
	{
		if (BlasterHUD->EndGame == nullptr) BlasterHUD->AddEndGame();
		if (BlasterHUD->EndGame)
		{
			BlasterHUD->EndGame->ShowEndGameResult(WinnerTeam);
		}
		DisableInput(this);

	}
}


FString ABlasterPlayerController::GetInfoText(const TArray<class ABlasterPlayerState*>& Players)
{
	ABlasterPlayerState* BlasterPlayerState = GetPlayerState<ABlasterPlayerState>();
	if (BlasterPlayerState == nullptr) return FString();

	FString InfoTextString;
	if (Players.Num() == 0)
	{
		InfoTextString = Announcement::ThereIsNoWinner;
	}
	else if (Players.Num() == 1 && Players[0] == BlasterPlayerState)
	{
		InfoTextString = Announcement::YouAreTheWinner;

	}
	else if (Players.Num() == 1)
	{
		InfoTextString = FString::Printf(TEXT("El l�der es %s"), *Players[0]->GetPlayerName());
	}
	else if (Players.Num() > 1)
	{
		InfoTextString = Announcement::PlayersTiedForTheWin;
		InfoTextString.Append(FString("\n"));
		for (auto TiedPlayer : Players)
		{
			InfoTextString.Append(FString::Printf(TEXT("%s\n"), *TiedPlayer->GetPlayerName()));
		}
	}

	return InfoTextString;
}

FString ABlasterPlayerController::GetTeamsInfoText(ABlasterGameState* BlasterGameState)
{
	if (BlasterGameState == nullptr) 
	{
		return FString();

	}
	FString InfoTextString;
	UE_LOG(LogTemp, Warning, TEXT("LLAMADO PARA SOLICITAR EL DE TEAMS"));

	int32 RedTeamScore = BlasterGameState->RedTeamScore;
	int32 BlueTeamScore = BlasterGameState->BlueTeamScore;
	UE_LOG(LogTemp, Warning, TEXT("Valores: RED = %d | BLUE = %d"), RedTeamScore,BlueTeamScore);

	if (RedTeamScore == 0 && BlueTeamScore == 0)
	{
		InfoTextString = Announcement::ThereIsNoWinner;//Empate
	}
	else if (RedTeamScore == BlueTeamScore) //Empate
	{
		InfoTextString = FString::Printf(TEXT("%s\n"), *Announcement::TeamsTiedForTheWin);
		InfoTextString.Append(Announcement::RedTeam);
		InfoTextString.Append(TEXT("\n"));
		InfoTextString.Append(Announcement::BlueTeam);
		InfoTextString.Append(TEXT("\n"));
	}
	else if (RedTeamScore > BlueTeamScore)
	{
		InfoTextString = Announcement::RedTeamWins;
		InfoTextString.Append(TEXT("\n"));
		InfoTextString.Append(FString::Printf(TEXT("%s: %d\n"), *Announcement::RedTeam, (int)BlasterGameState->RedTeamRoundScore + 1));
		InfoTextString.Append(FString::Printf(TEXT("%s: %d\n"), *Announcement::BlueTeam, (int)BlasterGameState->BlueTeamRoundScore));
	}
	else if (BlueTeamScore > RedTeamScore)
	{
		InfoTextString = Announcement::BlueTeamWins;
		InfoTextString.Append(TEXT("\n"));
		InfoTextString.Append(FString::Printf(TEXT("%s: %d\n"), *Announcement::BlueTeam, (int)BlasterGameState->BlueTeamRoundScore + 1));
		InfoTextString.Append(FString::Printf(TEXT("%s: %d\n"), *Announcement::RedTeam, (int)BlasterGameState->RedTeamRoundScore));
	}

	return InfoTextString;
}
