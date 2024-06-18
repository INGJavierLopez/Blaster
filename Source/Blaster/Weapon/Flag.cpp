// Fill out your copyright notice in the Description page of Project Settings.


#include "Flag.h"
#include "Components/SphereComponent.h"
#include "Blaster/Character/BlasterCharacter.h"


AFlag::AFlag()
{
	GetWeaponMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetWeaponMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AFlag::Dropped()
{
	SetWeaponState(EWeaponState::EWS_Dropped);
	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
	GetWeaponMesh()->DetachFromComponent(DetachRules);
	SetOwner(nullptr);
	BlasterOwnerCharacter = nullptr;
	BlasterOwnerController = nullptr;
}

void AFlag::ResetFlag()
{
	

	ABlasterCharacter* FlagBearer = Cast<ABlasterCharacter>(GetOwner());
	if (FlagBearer)
	{
		FlagBearer->SetHoldingTheFlag(false);
		FlagBearer->SetOverlappingWeapon(nullptr);
		FlagBearer->UnCrouch();
	}

	if (!HasAuthority()) return;
	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
	GetWeaponMesh()->DetachFromComponent(DetachRules);

	SetWeaponState(EWeaponState::EWS_Initial);
	GetAreaSphere()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetAreaSphere()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	SetOwner(nullptr);
	BlasterOwnerCharacter = nullptr;
	BlasterOwnerController = nullptr;

	SetActorTransform(InitialTransform);
}

void AFlag::OnEquipped()
{
	ShowPickupWidget(false);
	GetAreaSphere()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetWeaponMesh()->SetSimulatePhysics(false);
	GetWeaponMesh()->SetEnableGravity(false);
	GetWeaponMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetWeaponMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	EnableCustomDepth(false);


}

void AFlag::OnDropped()
{
	if (HasAuthority())
	{
		GetAreaSphere()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	GetWeaponMesh()->SetSimulatePhysics(true);
	GetWeaponMesh()->SetWorldRotation(FRotator::ZeroRotator);
	GetWeaponMesh()->SetEnableGravity(true);
	GetWeaponMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetWeaponMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	GetWeaponMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	GetWeaponMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	GetWeaponMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_BLUE);
	GetWeaponMesh()->MarkRenderStateDirty();
	EnableCustomDepth(true);

}

void AFlag::BeginPlay()
{
	Super::BeginPlay();
	InitialTransform = GetActorTransform();
}
