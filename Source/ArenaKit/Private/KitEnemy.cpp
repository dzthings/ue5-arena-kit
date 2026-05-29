#include "KitEnemy.h"

#include "KitAIController.h"
#include "KitCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/FloatingPawnMovement.h"

AKitEnemy::AKitEnemy()
{
    PrimaryActorTick.bCanEverTick = false;

    // Capsule is the collision and nav root — independent of the visual mesh.
    Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
    Capsule->InitCapsuleSize(40.f, 80.f);
    Capsule->SetCollisionProfileName(TEXT("BlockAll"));
    Capsule->SetCanEverAffectNavigation(false);
    SetRootComponent(Capsule);

    // Visual only — no collision. Scale or swap this freely.
    BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
    BodyMesh->SetupAttachment(Capsule);
    BodyMesh->SetCollisionProfileName(TEXT("NoCollision"));
    BodyMesh->SetCanEverAffectNavigation(false);

    // Overlap-only sphere for contact damage — doesn't block or affect nav.
    DamageSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DamageSphere"));
    DamageSphere->SetupAttachment(Capsule);
    DamageSphere->InitSphereRadius(55.f);
    DamageSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    DamageSphere->SetCanEverAffectNavigation(false);

    MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
    MovementComponent->MaxSpeed = 300.f;

    AIControllerClass = AKitAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AKitEnemy::BeginPlay()
{
    Super::BeginPlay();

    CurrentHealth = MaxHealth;

    DamageSphere->OnComponentBeginOverlap.AddDynamic(this, &AKitEnemy::OnDamageSphereOverlap);
}

float AKitEnemy::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent,
    AController* EventInstigator, AActor* DamageCauser)
{
    const float Applied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    CurrentHealth = FMath::Max(CurrentHealth - Applied, 0.f);
    UE_LOG(LogTemp, Warning, TEXT("Enemy took %.0f damage — %.0f / %.0f HP remaining"), Applied, CurrentHealth, MaxHealth);

    if (CurrentHealth <= 0.f)
    {
        Destroy();
    }

    return Applied;
}

void AKitEnemy::OnDamageSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (bContactDamageOnCooldown)
    {
        return;
    }

    AKitCharacter* Player = Cast<AKitCharacter>(OtherActor);
    if (!Player)
    {
        return;
    }

    Player->TakeDamage(ContactDamage, FDamageEvent(), GetController(), this);

    bContactDamageOnCooldown = true;
    FTimerHandle CooldownTimer;
    GetWorldTimerManager().SetTimer(CooldownTimer, [this]()
    {
        bContactDamageOnCooldown = false;
    }, 0.75f, false);
}
