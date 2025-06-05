// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamScoreTab.h"
#include "Components/ScrollBox.h"
#include "ScoreTabSlot.h"
#include "Components/TextBlock.h"

void UTeamScoreTab::UpdateTables(const TArray<FScoreSlotInfo>& GroupA, const TArray<FScoreSlotInfo>& GroupB)
{
	if (BlueTeamSB == nullptr) return;
	if (RedTeamSB == nullptr) return;

	BlueTeamSB->ClearChildren();
	RedTeamSB->ClearChildren();

	for (FScoreSlotInfo SlotInfo : GroupA)
	{
		UScoreTabSlot* ScoreTabSlot = CreateWidget<UScoreTabSlot>(GetWorld(), TeamScoreSlotClass);
		if (ScoreTabSlot)
		{
			ScoreTabSlot->PlayerNameText->SetText(FText::FromString(SlotInfo.PlayerName));
			ScoreTabSlot->KillsText->SetText(FText::FromString(SlotInfo.Kills));
			ScoreTabSlot->DeathsText->SetText(FText::FromString(SlotInfo.Deaths));
			BlueTeamSB->AddChild(ScoreTabSlot);
		}
	}
	//haz lo mismo para el RedTeamSB
	for (FScoreSlotInfo SlotInfo : GroupB)
	{
		UScoreTabSlot* ScoreTabSlot = CreateWidget<UScoreTabSlot>(GetWorld(), TeamScoreSlotClass);
		if (ScoreTabSlot)
		{
			ScoreTabSlot->PlayerNameText->SetText(FText::FromString(SlotInfo.PlayerName));
			ScoreTabSlot->KillsText->SetText(FText::FromString(SlotInfo.Kills));
			ScoreTabSlot->DeathsText->SetText(FText::FromString(SlotInfo.Deaths));
			RedTeamSB->AddChild(ScoreTabSlot);
		}
	}

}
