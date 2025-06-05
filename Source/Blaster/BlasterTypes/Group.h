#pragma once

UENUM(BlueprintType)
enum class EGroup : uint8
{
	EG_A UMETA(DisplayName = "GroupA"),
	EG_B UMETA(DisplayName = "GroupB"),
	EG_NG UMETA(DisplayName = "NoGroup"),
	EG_MAX UMETA(DisplayName = "DefaultMAX")
};