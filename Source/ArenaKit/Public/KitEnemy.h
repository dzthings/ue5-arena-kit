#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "KitEnemy.generated.h"

class UCapsuleComponent;
class UFloatingPawnMovement;
class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class ARENAKIT_API AKitEnemy : public APawn
{
    GENERATED_BODY()

public:
    AKitEnemy();

    virtual float TakeDamage(float DamageAmount, const struct FDamageEvent& DamageEvent,
        AController* EventInstigator, AActor* DamageCauser) override;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UCapsuleComponent> Capsule;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> BodyMesh;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UFloatingPawnMovement> MovementComponent;

    // Slightly larger than the capsule — fires overlap events to deal contact damage.
    // Separate from the blocking Capsule so it doesn't affect navigation or physics.
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USphereComponent> DamageSphere;

    UPROPERTY(EditAnywhere, Category = "Combat")
    float MaxHealth = 100.f;

    UPROPERTY(EditAnywhere, Category = "Combat")
    float ContactDamage = 10.f;

private:
    UFUNCTION()
    void OnDamageSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    float CurrentHealth;
    bool bContactDamageOnCooldown = false;
};
