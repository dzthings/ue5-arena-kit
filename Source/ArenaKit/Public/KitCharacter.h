#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "KitCharacter.generated.h"

class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
struct FInputActionValue;

UCLASS()
class ARENAKIT_API AKitCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AKitCharacter();

    virtual void Tick(float DeltaSeconds) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    virtual float TakeDamage(float DamageAmount, const struct FDamageEvent& DamageEvent,
        AController* EventInstigator, AActor* DamageCauser) override;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USpringArmComponent> SpringArm;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UCameraComponent> Camera;

    UPROPERTY(VisibleAnywhere, Category = "Input")
    TObjectPtr<UInputMappingContext> DefaultMappingContext;

    UPROPERTY(VisibleAnywhere, Category = "Input")
    TObjectPtr<UInputAction> MoveAction;

    UPROPERTY(VisibleAnywhere, Category = "Input")
    TObjectPtr<UInputAction> FireAction;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float MoveSpeed = 600.f;

    UPROPERTY(EditAnywhere, Category = "Combat")
    TSubclassOf<AActor> ProjectileClass;

    UPROPERTY(EditAnywhere, Category = "Combat")
    float MuzzleOffset = 80.f;

    UPROPERTY(EditAnywhere, Category = "Combat")
    float MaxHealth = 100.f;

    float CurrentHealth;

private:
    void Move(const FInputActionValue& Value);
    void Fire();
};
