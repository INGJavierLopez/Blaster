// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Blaster/BlasterTypes/Group.h"
#include "Blaster/BlasterTypes/Team.h"

#include "BlasterGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UBlasterGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
    // Mapa de NetID -> Información del Grupo y Equipo
    UPROPERTY(BlueprintReadWrite)
    TMap<FString, EGroup> PlayerGroupMap;

    UPROPERTY(BlueprintReadWrite)
    TMap<FString, ETeam> PlayerCurrentTeamMap;

    // Asigna un jugador a un grupo
    UFUNCTION(BlueprintCallable)
    void AssignPlayerToGroup(const FString& NetID, EGroup Group);

    UFUNCTION()
    void RemovePlayerFromGroup(const FString& NetID);

    // Obtiene el grupo de un jugador
    UFUNCTION(BlueprintCallable)
    EGroup GetPlayerGroup(const FString& NetID) const;

    // Asigna un equipo actual a un jugador
    UFUNCTION(BlueprintCallable)
    void AssignPlayerToCurrentTeam(const FString& NetID, ETeam Team);

    // Obtiene el equipo actual de un jugador
    UFUNCTION(BlueprintCallable)
    ETeam GetPlayerCurrentTeam(const FString& NetID) const;
};
