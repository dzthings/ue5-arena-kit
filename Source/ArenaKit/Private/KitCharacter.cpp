#include "KitCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "UObject/ConstructorHelpers.h"

AKitCharacter::AKitCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // Don't let the controller rotate the character — we aim via cursor.
    bUseControllerRotationYaw   = false;
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll  = false;

    UCharacterMovementComponent* CMC = GetCharacterMovement();
    CMC->bOrientRotationToMovement    = false;
    CMC->bUseControllerDesiredRotation = false;
    CMC->MaxWalkSpeed                 = MoveSpeed;

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(GetCapsuleComponent());
    SpringArm->TargetArmLength = 1200.f;
    SpringArm->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
    SpringArm->bDoCollisionTest = false;

    // Camera stays fixed; only the character mesh rotates toward the cursor.
    SpringArm->SetUsingAbsoluteRotation(true);
    SpringArm->bUsePawnControlRotation = false;
    SpringArm->bInheritPitch           = false;
    SpringArm->bInheritYaw             = false;
    SpringArm->bInheritRoll            = false;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
    Camera->bUsePawnControlRotation = false;

    // Load input assets by path so Blueprint reparents can never null them out.
    static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCFinder(
        TEXT("/Game/Input/IMC_Default"));
    if (IMCFinder.Succeeded()) DefaultMappingContext = IMCFinder.Object;

    static ConstructorHelpers::FObjectFinder<UInputAction> MoveFinder(
        TEXT("/Game/Input/Actions/IA_Move"));
    if (MoveFinder.Succeeded()) MoveAction = MoveFinder.Object;

    static ConstructorHelpers::FObjectFinder<UInputAction> FireFinder(
        TEXT("/Game/Input/IA_Fire"));
    if (FireFinder.Succeeded()) FireAction = FireFinder.Object;
}

void AKitCharacter::BeginPlay()
{
    Super::BeginPlay();

    CurrentHealth = MaxHealth;

    APlayerController* PC = Cast<APlayerController>(GetController());
    if (!PC)
    {
        return;
    }

    PC->SetShowMouseCursor(true);
    PC->SetInputMode(FInputModeGameOnly());

    UEnhancedInputLocalPlayerSubsystem* Subsystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());

    if (Subsystem && DefaultMappingContext)
    {
        Subsystem->AddMappingContext(DefaultMappingContext, 0);
    }
}

void AKitCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    APlayerController* PC = Cast<APlayerController>(GetController());
    if (!PC)
    {
        return;
    }

    float MouseX = 0.f;
    float MouseY = 0.f;
    if (!PC->GetMousePosition(MouseX, MouseY))
    {
        return;
    }

    FVector WorldOrigin;
    FVector WorldDirection;
    if (!PC->DeprojectScreenPositionToWorld(MouseX, MouseY, WorldOrigin, WorldDirection))
    {
        return;
    }

    // Intersect cursor ray with the horizontal plane at the character's feet.
    if (FMath::IsNearlyZero(WorldDirection.Z))
    {
        return;
    }

    const float T = (GetActorLocation().Z - WorldOrigin.Z) / WorldDirection.Z;
    if (T < 0.f)
    {
        return;
    }

    const FVector CursorWorld = WorldOrigin + WorldDirection * T;

    FVector ToTarget = CursorWorld - GetActorLocation();
    ToTarget.Z = 0.f;

    if (ToTarget.SizeSquared() < 1.f)
    {
        return;
    }

    // Rotate the whole character to face the cursor; camera is decoupled (absolute rotation).
    SetActorRotation(FRotator(0.f, ToTarget.Rotation().Yaw, 0.f));
}

void AKitCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    if (!EIC)
    {
        return;
    }

    if (MoveAction)
    {
        EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AKitCharacter::Move);
    }

    if (FireAction)
    {
        EIC->BindAction(FireAction, ETriggerEvent::Started, this, &AKitCharacter::Fire);
    }
}

float AKitCharacter::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent,
    AController* EventInstigator, AActor* DamageCauser)
{
    const float Applied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    CurrentHealth = FMath::Max(CurrentHealth - Applied, 0.f);
    UE_LOG(LogTemp, Warning, TEXT("Player took %.0f damage — %.0f / %.0f HP remaining"),
        Applied, CurrentHealth, MaxHealth);

    if (CurrentHealth <= 0.f)
    {
        UE_LOG(LogTemp, Warning, TEXT("Player died."));
    }

    return Applied;
}

void AKitCharacter::Move(const FInputActionValue& Value)
{
    const FVector2D Axis = Value.Get<FVector2D>();
    AddMovementInput(FVector::ForwardVector, Axis.Y);
    AddMovementInput(FVector::RightVector,   Axis.X);
}

void AKitCharacter::Fire()
{
    if (!ProjectileClass)
    {
        return;
    }

    // Prefer a named socket on the skeletal mesh ("muzzle"); fall back to forward offset.
    const USkeletalMeshComponent* SkelMesh = GetMesh();
    const FVector MuzzleLocation = (SkelMesh && SkelMesh->DoesSocketExist(FName("muzzle")))
        ? SkelMesh->GetSocketLocation(FName("muzzle"))
        : GetActorLocation() + GetActorForwardVector() * MuzzleOffset;

    const FRotator SpawnRotation = GetActorRotation();

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner     = this;
    SpawnParams.Instigator = this;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, SpawnRotation, SpawnParams);
}
