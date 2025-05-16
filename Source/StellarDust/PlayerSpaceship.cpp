// PlayerSpaceship.cpp
#include "PlayerSpaceship.h"

APlayerSpaceship::APlayerSpaceship()
{
    PrimaryActorTick.bCanEverTick = true;
    
    CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
    SetRootComponent(CapsuleComp);

    PlayerSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PlayerSprite"));
    PlayerSprite->SetupAttachment(RootComponent);
    
    CenterSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("CenterSpawn"));
    CenterSpawn->SetupAttachment(PlayerSprite);
    
    LeftSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("LeftSpawn"));
    LeftSpawn->SetupAttachment(PlayerSprite);
    
    RightSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("RightSpawn"));
    RightSpawn->SetupAttachment(PlayerSprite);

}

void APlayerSpaceship::BeginPlay()
{
    Super::BeginPlay();
    
    APlayerController* PlayerController = Cast<APlayerController>(Controller);
    if (PlayerController){
        UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
        if(Subsystem){
            Subsystem->AddMappingContext(IMC_Spaceship, 0);
        }
    }
}

void APlayerSpaceship::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (MovementDirection.IsNearlyZero())
            return;
    
    
    FVector2D DistanceToMove = MovementDirection * MovementSpeed * DeltaTime;
    FVector CurrentLocation = GetActorLocation();
    FVector NewLocation = CurrentLocation + FVector(DistanceToMove.X, 0.0f, 0.0f);
    
    if (!IsInMapBoundsHorizontal(NewLocation.X)){
        NewLocation -= FVector(DistanceToMove.X, 0.0f, 0.0f);
    }
    NewLocation += FVector(0.0f, 0.0f, DistanceToMove.Y);
    if (!IsInMapBoundsVertical(NewLocation.Z)){
        NewLocation -= FVector(0.0f, 0.0f, DistanceToMove.Y);
    }
    SetActorLocation(NewLocation, true);

}

bool APlayerSpaceship::IsInMapBoundsHorizontal(float Xpos)
{
    bool Result = true;
    Result = (Xpos > HorizontalLimits.X) && (Xpos < HorizontalLimits.Y);
    return Result;
}

bool APlayerSpaceship::IsInMapBoundsVertical(float Zpos)
{
    bool Result = true;
    Result = (Zpos > VerticalLimits.X) && (Zpos < VerticalLimits.Y);
    return Result;
}

void APlayerSpaceship::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
    
    if(EnhancedInputComponent){
        EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APlayerSpaceship::MoveTriggered);
        EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Completed, this, &APlayerSpaceship::MoveCompleted);
        EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Canceled, this, &APlayerSpaceship::MoveCompleted);
        
//        EnhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Started, this, &APlayerSpaceship::Shoot);
        EnhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Triggered, this, &APlayerSpaceship::Shoot);
        }
}

void APlayerSpaceship::MoveTriggered(const FInputActionValue& Value){
    
    FVector2D DeltaXY = Value.Get<FVector2D>();
    MovementDirection = DeltaXY;
}

void APlayerSpaceship::MoveCompleted(const FInputActionValue& Value){
    
    MovementDirection = FVector2D(0.0f, 0.0f);
}

void APlayerSpaceship::Shoot(const FInputActionValue& Value){
    if(CanShoot){
        CanShoot = false;
        
        TArray<USceneComponent*> Points;
            switch (UpgradeLevel)
            {
                case 2:
                    Points = { LeftSpawn, RightSpawn };
                    break;

                case 3:
                    Points = { CenterSpawn, LeftSpawn, RightSpawn };
                    break;

                default: // Level 1
                    Points = { CenterSpawn };
                    break;
            }
        
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        for (auto* P : Points)
            {
                FVector Loc = P->GetComponentLocation();
                ABlueBullet* B = GetWorld()->SpawnActor<ABlueBullet>(BulletClass, Loc, FRotator::ZeroRotator, SpawnParams);
                if (B) B->Launch();
            }
        
        
        GetWorldTimerManager().SetTimer(ShootCooldownTimer, this, &APlayerSpaceship:: OnShootCooldownTimerTimeout, 1.0f, false, ShootCooldownInSec);
        
    }
    
    
}

void APlayerSpaceship::OnShootCooldownTimerTimeout(){
    CanShoot = true;
    
}
