// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectTeam.h"
#include "Components/Button.h"
#include "Blaster/PlayerController/GameStartupPlayerController.h"
#include "Blaster/BlasterTypes/Group.h"

void USelectTeam::NativeConstruct()
{
	Super::NativeConstruct();
	if (JoinGroupA)
	{
		JoinGroupA->OnClicked.AddDynamic(this, &USelectTeam::JoinGroupACkicked);
		JoinGroupA->IsFocusable = false;
	}
	if (JoinGroupB)
	{
		JoinGroupB->OnClicked.AddDynamic(this, &USelectTeam::JoinGroupBCkicked);
		JoinGroupB->IsFocusable = false;

	}
}

void USelectTeam::JoinGroupACkicked()
{
	AGameStartupPlayerController* BlasterPlayerController = Cast<AGameStartupPlayerController>(GetOwningPlayer());
	if (BlasterPlayerController)
	{
		BlasterPlayerController->JoinToGroup(EGroup::EG_A);
	}
}

void USelectTeam::JoinGroupBCkicked()
{
	AGameStartupPlayerController* BlasterPlayerController = Cast<AGameStartupPlayerController>(GetOwningPlayer());
	if (BlasterPlayerController)
	{
		BlasterPlayerController->JoinToGroup(EGroup::EG_B);
	}
}
