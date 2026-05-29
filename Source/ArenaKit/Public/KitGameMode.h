#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "KitGameMode.generated.h"

UCLASS()
class ARENAKIT_API AKitGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AKitGameMode();

    // Called by AKitCharacter when the player's health reaches zero.
    void NotifyPlayerDied();

    // Seconds between death and level restart. Tune in BP_KitGameMode.
    UPROPERTY(EditDefaultsOnly, Category = "Arena")
    float RestartDelay = 2.f;

private:
    void RestartLevel();

    FTimerHandle RestartTimerHandle;
};
