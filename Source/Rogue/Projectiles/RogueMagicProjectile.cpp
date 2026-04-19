// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueMagicProjectile.h"

#include "NiagaraFunctionLibrary.h"
#include "../../../../../../../Program Files/Epic Games/UE_5.7/Engine/Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


ARogueMagicProjectile::ARogueMagicProjectile()
{
	SphereColliderComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	SphereColliderComponent->SetSphereRadius(16.0f);
	SphereColliderComponent->SetCollisionProfileName("RogueProjectile");
	RootComponent = SphereColliderComponent;
	
	PrimaryParticlesComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("PrimaryParticles"));
	PrimaryParticlesComponent->SetupAttachment(SphereColliderComponent);
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->InitialSpeed = 1500.0f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}

void ARogueMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereColliderComponent->OnComponentHit.AddDynamic(this, &ARogueMagicProjectile::OnParticleHit);
	SphereColliderComponent->IgnoreActorWhenMoving(GetInstigator(), true);
}

void ARogueMagicProjectile::OnParticleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	TSubclassOf<UDamageType> DamageTypeClass = UDamageType::StaticClass();
	UGameplayStatics::ApplyDamage(OtherActor, 10.0f, GetInstigatorController(), this, DamageTypeClass);
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ExplosionEffectSystem, GetActorLocation());
	
	Destroy();
}
