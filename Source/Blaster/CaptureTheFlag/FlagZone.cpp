// Fill out your copyright notice in the Description page of Project Settings.


#include "FlagZone.h"
#include "Components/SphereComponent.h"
#include "Blaster/Weapon/Flag.h"
#include "Blaster/GameMode/CaptureTheFlagGameMode.h"
#include "Blaster/Character/BlasterCharacter.h"

AFlagZone::AFlagZone()
{
	PrimaryActorTick.bCanEverTick = false;

	ZoneSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ZoneSphere"));

}

void AFlagZone::BeginPlay()
{
	Super::BeginPlay();

	ZoneSphere->OnComponentBeginOverlap.AddDynamic(this, &AFlagZone::OnSphereOverlap);
	
}

void AFlagZone::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OVerlap"));
	
	ABlasterCharacter* OverlappingPlayer = Cast<ABlasterCharacter>(OtherActor);
	if (OverlappingPlayer &&OverlappingPlayer->GetFlag())
	{
		UE_LOG(LogTemp, Warning, TEXT("Take Character"));
		AFlag* OverlappingFlag = Cast<AFlag>(OverlappingPlayer->GetFlag());
		if(OverlappingFlag) UE_LOG(LogTemp, Warning, TEXT("Take Flag"));
		if (OverlappingFlag && OverlappingFlag->GetTeam() != Team)
		{

			ACaptureTheFlagGameMode* GameMode = GetWorld()->GetAuthGameMode<ACaptureTheFlagGameMode>();
			if (GameMode)
			{
				GameMode->FlagCaptured(OverlappingFlag, this);

			}
			OverlappingFlag->ResetFlag();
		}
	}
	
}


