// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class BLASTER_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,const FHitResult& Hit);

	UPROPERTY(EditAnywhere)
	float Damage = 20;

	bool bHitCharacter;
	//este es el segundo
	UFUNCTION(NetMulticast, Reliable)
	void MulticastHit(FVector HitLocation);
private:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;
	
	UPROPERTY(EditAnywhere)
	class UParticleSystem* Tracer;

	UPROPERTY()
	class UParticleSystemComponent* TracerComponent;

	//Particles
	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactParticles;

	//Sound
	UPROPERTY(EditAnywhere)
	class USoundCue* BodyImpactSound;
	UPROPERTY(EditAnywhere)
	class USoundCue* TerrainImpactSound;
	class USoundCue* ImpactSound;
	FTimerHandle TimerDestroy;
	float DestroyDelay = 3.f;

public:	

};
