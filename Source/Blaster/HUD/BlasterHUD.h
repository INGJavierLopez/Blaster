// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BlasterHUD.generated.h"

USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()
public:
	class UTexture2D* CrosshairsCenter;
	UTexture2D* CrosshairsLeft;
	UTexture2D* CrosshairsRight;
	UTexture2D* CrosshairsTop;
	UTexture2D* CrosshairsBottom;
	float CrosshairSpread;
	FLinearColor CrosshairsColor;
};

/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void DrawHUD() override;

	/////////////CHARACTER OVERLAY
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	TSubclassOf<class UUserWidget> CharacterOverlayClass;
	void AddCharacterOverlay();

	UPROPERTY()
	class UCharacterOverlay* CharacterOverlay;

	////////// ANNOUNCEMENT
	UPROPERTY(EditAnywhere, Category = "Announcements")
	TSubclassOf<UUserWidget> AnnouncementClass;

	UPROPERTY()
	class UAnnouncement* Announcement;

	void AddAnnouncement();
	void AddElimAnnouncement(FString ElimText);
	void LoadAnnouncement();

	////////////// SELECT TEAM TAB
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	TSubclassOf<class UUserWidget> SelectTeamClass;

	class USelectTeam* SelectTeam;
	void AddSelectTeam();

	//////////////  SCORE TAB
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	TSubclassOf<class UUserWidget> ScoreTabClass;

	class UScoreTab* ScoreTab;
	void AddScoreTab();

	////////////// TEAM Score Tab
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	TSubclassOf<class UUserWidget> TeamScoreTabClass;

	class UTeamScoreTab* TeamScoreTab;
	void AddTeamScoreTab();

	//////////////  LOBBY HUD
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	TSubclassOf<class UUserWidget> LobbyHUDClass;
	UPROPERTY()
	class ULobbyHUD* LobbyHUD;
	void AddLobbyHUD();


	////////////// NEW ROUND
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	TSubclassOf<class UUserWidget> NewRoundClass;

	class UNewRound* NewRound;
	void AddNewRound();

	/////////////// END ROUND
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	TSubclassOf<class UUserWidget> EndRoundClass;

	UPROPERTY()
	class UEndRound* EndRound;
	void AddEndRound();

	////////////////EndGame
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	TSubclassOf<class UUserWidget> EndGameClass;

	//UPROPERTY()
	class UEndGame* EndGame;
	void AddEndGame();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	APlayerController* OwningPlayer;

	FHUDPackage HUDPackage;

	void DrawCrosshair(UTexture2D* Texture,FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairColor);

	UPROPERTY(EditAnywhere)
	float CrosshairSpreadMax = 16.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UElimAnnouncement> ElimAnnouncementClass;

	UPROPERTY(EditAnywhere)
	float ElimAnnouncementTime = 2.5f;

	UFUNCTION()
	void ElimAnnouncementTimerFinished(UElimAnnouncement* MSGToRemove);
	UPROPERTY()
	TArray<UElimAnnouncement*> ElimMessages;
public:
	FORCEINLINE void SetHUDPackage(const FHUDPackage& Package) { HUDPackage = Package; }
};
