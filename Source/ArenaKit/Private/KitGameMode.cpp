#include "KitGameMode.h"

#include "Kismet/GameplayStatics.h"

AKitGameMode::AKitGameMode()
{
}

void AKitGameMode::NotifyPlayerDied()
{
    // Guard against multiple calls if the player somehow takes damage twice at 0 HP.
    if (GetWorldTimerManager().IsTimerActive(RestartTimerHandle))
    {
        return;
    }

    GetWorldTimerManager().SetTimer(RestartTimerHandle, this, &AKitGameMode::RestartLevel, RestartDelay, false);
}

void AKitGameMode::RestartLevel()
{
    UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()));
}
