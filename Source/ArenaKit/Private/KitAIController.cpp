#include "KitAIController.h"

#include "Kismet/GameplayStatics.h"

void AKitAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    GetWorldTimerManager().SetTimer(
        MovementTimerHandle,
        this,
        &AKitAIController::UpdateMovement,
        0.25f,
        true
    );
}

void AKitAIController::UpdateMovement()
{
    if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0))
    {
        MoveToActor(PlayerPawn, 50.f);
    }
}
