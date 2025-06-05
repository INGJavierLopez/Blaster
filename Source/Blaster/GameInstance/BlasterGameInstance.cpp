// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterGameInstance.h"

void UBlasterGameInstance::AssignPlayerToGroup(const FString& NetID, EGroup Group)
{
	if (!NetID.IsEmpty())
	{
		// Verificar si el jugador ya está en un grupo
		if (PlayerGroupMap.Contains(NetID))
		{
			PlayerGroupMap.Remove(NetID); // Eliminar al jugador de su grupo actual
		}

		// Asignar el nuevo grupo
		PlayerGroupMap.Add(NetID, Group);
	}
}

void UBlasterGameInstance::RemovePlayerFromGroup(const FString& NetID)
{
	if (!NetID.IsEmpty() && PlayerGroupMap.Contains(NetID))
	{
		PlayerGroupMap.Remove(NetID);
	}
}

EGroup UBlasterGameInstance::GetPlayerGroup(const FString& NetID) const
{
	const EGroup* Group = PlayerGroupMap.Find(NetID);
	return Group ? *Group : EGroup::EG_NG;
}

void UBlasterGameInstance::AssignPlayerToCurrentTeam(const FString& NetID, ETeam Team)
{
	if (!NetID.IsEmpty())
	{
		PlayerCurrentTeamMap.Add(NetID, Team);
	}
}

ETeam UBlasterGameInstance::GetPlayerCurrentTeam(const FString& NetID) const
{
	const ETeam* Team = PlayerCurrentTeamMap.Find(NetID);
	return Team ? *Team : ETeam::ET_NoTeam;
}
