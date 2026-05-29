#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "KitEnemySpawner.generated.h"

UCLASS()
class ARENAKIT_API AKitEnemySpawner : public AActor
{
    GENERATED_BODY()

public:
    AKitEnemySpawner();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USceneComponent> Root;

    // Enemy class to spawn. Set to BP_KitEnemy (or a subclass) in the level.
    UPROPERTY(EditAnywhere, Category = "Spawner")
    TSubclassOf<APawn> EnemyClass;

    // How many enemies to keep alive. When one dies a replacement spawns after SpawnInterval.
    UPROPERTY(EditAnywhere, Category = "Spawner")
    int32 SpawnCount = 3;

    // Seconds between a death and the replacement spawn.
    UPROPERTY(EditAnywhere, Category = "Spawner")
    float SpawnInterval = 2.f;

    // Radius around this actor within which enemies spawn (random point on the NavMesh).
    UPROPERTY(EditAnywhere, Category = "Spawner")
    float SpawnRadius = 300.f;

private:
    void SpawnEnemy();

    UFUNCTION()
    void OnEnemyDestroyed(AActor* DestroyedActor);

    int32 LiveEnemyCount = 0;
};
