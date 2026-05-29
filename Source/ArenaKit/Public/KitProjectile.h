#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KitProjectile.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;

UCLASS()
class ARENAKIT_API AKitProjectile : public AActor
{
    GENERATED_BODY()

public:
    AKitProjectile();

protected:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USphereComponent> CollisionSphere;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> Mesh;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UProjectileMovementComponent> Movement;

    UPROPERTY(EditAnywhere, Category = "Combat")
    float Damage = 25.f;

private:
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
               UPrimitiveComponent* OtherComp, FVector NormalImpulse,
               const FHitResult& Hit);
};
