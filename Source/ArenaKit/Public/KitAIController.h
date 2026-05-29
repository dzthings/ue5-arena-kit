#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "KitAIController.generated.h"

UCLASS()
class ARENAKIT_API AKitAIController : public AAIController
{
    GENERATED_BODY()

protected:
    virtual void OnPossess(APawn* InPawn) override;

private:
    void UpdateMovement();

    FTimerHandle MovementTimerHandle;
};
