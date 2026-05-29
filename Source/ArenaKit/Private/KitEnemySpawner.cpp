#include "KitEnemySpawner.h"

#include "NavigationSystem.h"

AKitEnemySpawner::AKitEnemySpawner()
{
    PrimaryActorTick.bCanEverTick = false;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);
}

void AKitEnemySpawner::BeginPlay()
{
    Super::BeginPlay();

    for (int32 i = 0; i < SpawnCount; ++i)
    {
        SpawnEnemy();
    }
}

void AKitEnemySpawner::SpawnEnemy()
{
    if (!EnemyClass)
    {
        return;
    }

    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
    FNavLocation NavLoc;
    const bool bFound = NavSys && NavSys->GetRandomReachablePointInRadius(GetActorLocation(), SpawnRadius, NavLoc);
    const FVector SpawnLocation = bFound ? NavLoc.Location : GetActorLocation();

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    APawn* Enemy = GetWorld()->SpawnActor<APawn>(EnemyClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
    if (!Enemy)
    {
        return;
    }

    ++LiveEnemyCount;

    // When the enemy dies, schedule a replacement after SpawnInterval.
    Enemy->OnDestroyed.AddDynamic(this, &AKitEnemySpawner::OnEnemyDestroyed);
}

void AKitEnemySpawner::OnEnemyDestroyed(AActor* DestroyedActor)
{
    --LiveEnemyCount;

    FTimerHandle SpawnTimer;
    GetWorldTimerManager().SetTimer(SpawnTimer, this, &AKitEnemySpawner::SpawnEnemy, SpawnInterval, false);
}
