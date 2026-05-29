#include "KitProjectile.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"

AKitProjectile::AKitProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    CollisionSphere->InitSphereRadius(15.f);
    CollisionSphere->SetCollisionProfileName(TEXT("BlockAllDynamic"));
    CollisionSphere->OnComponentHit.AddDynamic(this, &AKitProjectile::OnHit);
    SetRootComponent(CollisionSphere);

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(CollisionSphere);
    Mesh->SetCollisionProfileName(TEXT("NoCollision"));

    Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
    Movement->InitialSpeed = 1500.f;
    Movement->MaxSpeed = 1500.f;
    Movement->ProjectileGravityScale = 0.f;

    InitialLifeSpan = 3.f;
}

void AKitProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor && OtherActor != GetInstigator())
    {
        OtherActor->TakeDamage(Damage, FDamageEvent(), GetInstigatorController(), this);
    }
    Destroy();
}
