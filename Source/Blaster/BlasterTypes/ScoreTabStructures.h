
#pragma once

#include "CoreMinimal.h"
#include "ScoreTabStructures.generated.h"

USTRUCT(BlueprintType)
struct FScoreSlotInfo
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString PlayerName;

    UPROPERTY(BlueprintReadOnly)
    FString Kills;

    UPROPERTY(BlueprintReadOnly)
    FString Deaths;

    UPROPERTY(BlueprintReadOnly)
    FString Ping;
    // Sobrecarga del operador ==
    bool operator==(const FScoreSlotInfo& Other) const
    {
        return PlayerName == Other.PlayerName &&
            Kills == Other.Kills &&
            Deaths == Other.Deaths;
    }
};
