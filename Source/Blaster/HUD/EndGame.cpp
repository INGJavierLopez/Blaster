// Fill out your copyright notice in the Description page of Project Settings.


#include "EndGame.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"


void UEndGame::NativeOnInitialized()
{
	RedTeamColor = FLinearColor(.71f, 0.0f, 0.0f);
	BlueTeamColor = FLinearColor(.0f, 0.396f, 0.715f);
}

void UEndGame::SynchronizeProperties()
{
	Super::SynchronizeProperties();
}

void UEndGame::ShowEndGameResult(ETeam WinnerTeam)
{
	FString EndGameResult;
	switch (WinnerTeam)
	{
	case ETeam::ET_RedTeam:
		EndGameResult = FString::Printf(TEXT("Victoria \n Equipo Rojo"));
		FinalWinnerText->SetText(FText::FromString(EndGameResult));
		TeamWinImage->SetColorAndOpacity(RedTeamColor);
		break;
	case ETeam::ET_BlueTeam:
		EndGameResult = FString::Printf(TEXT("Victoria \n Equipo Azul"));
		FinalWinnerText->SetText(FText::FromString(EndGameResult));
		TeamWinImage->SetColorAndOpacity(BlueTeamColor);
		break;
	}
	
}
