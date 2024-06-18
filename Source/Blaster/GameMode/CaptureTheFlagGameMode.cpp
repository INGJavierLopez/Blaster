
#include "CaptureTheFlagGameMode.h"
#include "Blaster/Weapon/Flag.h"
#include "Blaster/CaptureTheFlag/FlagZone.h"
#include "Blaster/GameState/BlasterGameState.h"
#include "Kismet/GameplayStatics.h"

void ACaptureTheFlagGameMode::PlayerEliminated(class ABlasterCharacter* ElimmedCharacter, class ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController)
{
	ABlasterGameMode::PlayerEliminated(ElimmedCharacter, VictimController, AttackerController); //handle players score

}

void ACaptureTheFlagGameMode::FlagCaptured(AFlag* Flag, AFlagZone* Zone)
{
	bool bValidCapture = Flag->GetTeam() != Zone->Team;
	ABlasterGameState* BGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));
	if (BGameState)
	{
		if (Zone->Team == ETeam::ET_BlueTeam)
		{
			BGameState->BlueTeamScores();
		}
		if (Zone->Team == ETeam::ET_RedTeam)
		{
			BGameState->RedTeamScores();
		}
	}

}
