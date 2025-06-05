;// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterHUD.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "CharacterOverlay.h"
#include "Announcement.h"
#include "NewRound.h"
#include "EndRound.h"
#include "EndGame.h"
#include "SelectTeam.h"
#include "ScoreTab.h"
#include "TeamScoreTab.h"
#include "ElimAnnouncement.h"
#include "Components/HorizontalBox.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "LobbyHUD.h"

void ABlasterHUD::DrawHUD()
{
	Super::DrawHUD();
	ABlasterPlayerController* PlayerController = Cast<ABlasterPlayerController>(GetOwningPlayerController());
	if (PlayerController)
	{
		if (!PlayerController->IsMatchInProgress()) return;
	}
	FVector2D ViewportSize;
	if (GEngine)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		const FVector2D ViewportCenter(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);

		float SpreadScaled = CrosshairSpreadMax * HUDPackage.CrosshairSpread;

		if (HUDPackage.CrosshairsCenter)
		{
			FVector2D Spread(0.f, 0.f);
			DrawCrosshair(HUDPackage.CrosshairsCenter, ViewportCenter, Spread,HUDPackage.CrosshairsColor);
		}
		if (HUDPackage.CrosshairsLeft)
		{
			FVector2D Spread(-SpreadScaled, 0.f);
			DrawCrosshair(HUDPackage.CrosshairsLeft, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
		}
		if (HUDPackage.CrosshairsRight)
		{
			FVector2D Spread(SpreadScaled, 0.f);
			DrawCrosshair(HUDPackage.CrosshairsRight, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
		}
		if (HUDPackage.CrosshairsTop)
		{
			FVector2D Spread(0.f, SpreadScaled);
			DrawCrosshair(HUDPackage.CrosshairsTop, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
		}
		if (HUDPackage.CrosshairsBottom)
		{
			FVector2D Spread(0.f , -SpreadScaled);
			DrawCrosshair(HUDPackage.CrosshairsBottom, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
		}
	}
}







void ABlasterHUD::BeginPlay()
{
	Super::BeginPlay();
}

void ABlasterHUD::AddCharacterOverlay()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && CharacterOverlayClass)
	{
		CharacterOverlay = CreateWidget<UCharacterOverlay>(PlayerController, CharacterOverlayClass);
		CharacterOverlay->AddToViewport();
	}
}


void ABlasterHUD::AddAnnouncement()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && AnnouncementClass)
	{
		Announcement = CreateWidget<UAnnouncement>(PlayerController, AnnouncementClass);
		Announcement->AddToViewport();
	}
}

void ABlasterHUD::AddSelectTeam()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && SelectTeamClass)
	{
		SelectTeam = CreateWidget<USelectTeam>(PlayerController, SelectTeamClass);
		SelectTeam->AddToViewport();
		SelectTeam->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ABlasterHUD::AddScoreTab()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && ScoreTabClass)
	{
		ScoreTab = CreateWidget<UScoreTab>(PlayerController, ScoreTabClass);
		if (ScoreTab)
		{
			ScoreTab->AddToViewport();
		}
	}
}

void ABlasterHUD::AddTeamScoreTab()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && TeamScoreTabClass)
	{
		TeamScoreTab = CreateWidget<UTeamScoreTab>(PlayerController, TeamScoreTabClass);
		if (TeamScoreTab)
		{
			TeamScoreTab->AddToViewport();
			TeamScoreTab->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
void ABlasterHUD::AddLobbyHUD()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && LobbyHUDClass)
	{
		LobbyHUD = CreateWidget<ULobbyHUD>(PlayerController, LobbyHUDClass);
		if (LobbyHUD)
		{
			LobbyHUD->AddToViewport();
		}
	}
}
void ABlasterHUD::AddNewRound()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && NewRoundClass)
	{
		NewRound = CreateWidget<UNewRound>(PlayerController, NewRoundClass);
		if (NewRound)
		{
			NewRound->AddToViewport();

		}
	}
}
void ABlasterHUD::AddEndRound()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && EndRoundClass)
	{
		EndRound = CreateWidget<UEndRound>(PlayerController, EndRoundClass);
		if (EndRound)
		{
			EndRound->AddToViewport();

		}
	}
}

void ABlasterHUD::AddEndGame()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && EndGameClass)
	{
		EndGame = CreateWidget<UEndGame>(PlayerController, EndGameClass);
		if (EndGame)
		{
			EndGame->AddToViewport();
			EndGame->SetVisibility(ESlateVisibility::Visible);

		}
	}
}

void ABlasterHUD::AddElimAnnouncement(FString ElimText)
{
	OwningPlayer = OwningPlayer == nullptr ? GetOwningPlayerController() : OwningPlayer;

	if (OwningPlayer && ElimAnnouncementClass)
	{
		UElimAnnouncement* ElimAnnouncementWidget = CreateWidget<UElimAnnouncement>(OwningPlayer, ElimAnnouncementClass);
		if (ElimAnnouncementWidget)
		{
			ElimAnnouncementWidget->SetElimAnnouncementText(ElimText);
			ElimAnnouncementWidget->AddToViewport();

			for (UElimAnnouncement* Msg : ElimMessages)
			{
				if (Msg && Msg->AnnouncementBox)
				{
					UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Msg->AnnouncementBox);
					if (CanvasSlot)
					{
						FVector2D Position = CanvasSlot->GetPosition();
						FVector2D NewPosition(
							CanvasSlot->GetPosition().X,
							Position.Y - CanvasSlot->GetSize().Y
						);
						CanvasSlot->SetPosition(NewPosition);
					}
				}
			}

			ElimMessages.Add(ElimAnnouncementWidget);
			FTimerHandle ElimMSGTimer;
			FTimerDelegate ElimMSGDelegate;
			ElimMSGDelegate.BindUFunction(this, FName("ElimAnnouncementTimerFinished"), ElimAnnouncementWidget);
			GetWorldTimerManager().SetTimer(
				ElimMSGTimer,
				ElimMSGDelegate,
				ElimAnnouncementTime,
				false
			);
		}
	}
}

void ABlasterHUD::LoadAnnouncement()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	Announcement = CreateWidget<UAnnouncement>(PlayerController, AnnouncementClass);
	if (Announcement)
	{
		Announcement->SetVisibility(ESlateVisibility::Hidden);
	}
}



void ABlasterHUD::ElimAnnouncementTimerFinished(UElimAnnouncement* MSGToRemove)
{
	if (MSGToRemove)
	{
		MSGToRemove->RemoveFromParent();
	}
}

void ABlasterHUD::DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairColor)
{
	const float TextureWidth = Texture->GetSizeX();
	const float TextureHeight = Texture->GetSizeY();
	const FVector2D TextureDrawPoint(
		ViewportCenter.X - (TextureWidth / 2.f) + Spread.X,
		ViewportCenter.Y - (TextureHeight / 2.f + Spread.Y)
	);

	DrawTexture(
		Texture,
		TextureDrawPoint.X,
		TextureDrawPoint.Y,
		TextureWidth,
		TextureHeight,
		0.f,
		0.f,
		1.f,
		1.f,
		CrosshairColor
		);
}


