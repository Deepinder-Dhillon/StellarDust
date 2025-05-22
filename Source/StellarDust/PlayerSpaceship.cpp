#include "PlayerSpaceship.h"

APlayerSpaceship::APlayerSpaceship() {
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

void APlayerSpaceship::BeginPlay() {
    Super::BeginPlay();
    
    APlayerController* PlayerController = Cast<APlayerController>(Controller);
    if (PlayerController){
        UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
        
        if(Subsystem){
            Subsystem->AddMappingContext(IMC_Spaceship, 0);
        }
    }
}

void APlayerSpaceship::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
    
    if (MovementDirection.IsNearlyZero()) return;
    
    FVector2D DistanceToMove = MovementDirection * MovementSpeed * DeltaTime;
    FVector CurrentLocation = GetActorLocation();
    FVector NewLocation = CurrentLocation + FVector(DistanceToMove.X, 0.0f, 0.0f);
    
    if (!IsInMapBoundsHorizontal(NewLocation.X)) {
        NewLocation -= FVector(DistanceToMove.X, 0.0f, 0.0f);
    }
    
    NewLocation += FVector(0.0f, DistanceToMove.Y, 0.0f);
    
    if (!IsInMapBoundsVertical(NewLocation.Y)) {
        NewLocation -= FVector(0.0f, DistanceToMove.Y, 0.0f);
    }
    
    SetActorLocation(NewLocation, true);

}

bool APlayerSpaceship::IsInMapBoundsHorizontal(float Xpos) {
    bool Result = true;
    Result = (Xpos > HorizontalLimits.X) && (Xpos < HorizontalLimits.Y);
    return Result;
}

bool APlayerSpaceship::IsInMapBoundsVertical(float Ypos) {
    bool Result = true;
    Result = (Ypos > VerticalLimits.X) && (Ypos < VerticalLimits.Y);
    return Result;
}

void APlayerSpaceship::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
    
    if(EnhancedInputComponent){
        EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APlayerSpaceship::MoveTriggered);
        EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Completed, this, &APlayerSpaceship::MoveCompleted);
        EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Canceled, this, &APlayerSpaceship::MoveCompleted);

        EnhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Triggered, this, &APlayerSpaceship::Shoot);
        }
}

void APlayerSpaceship::MoveTriggered(const FInputActionValue& Value){
    
    FVector2D DeltaXY = Value.Get<FVector2D>();
    MovementDirection = DeltaXY;
}

void APlayerSpaceship::MoveCompleted(const FInputActionValue& Value) {
    MovementDirection = FVector2D(0.0f, 0.0f);
}

void APlayerSpaceship::Shoot(const FInputActionValue& Value){
    if(CanShoot){
        CanShoot = false;
        
        TArray<USceneComponent*> Points;
            switch (UpgradeLevel){
                    
                case 1:
                    Points = {CenterSpawn};
                    break;
                case 2:
                    Points = {LeftSpawn, RightSpawn};
                    break;
                default:
                    Points = {CenterSpawn, LeftSpawn, RightSpawn};
                    break;
            }
        
        for (auto* P : Points) {
                FVector Loc = P->GetComponentLocation();
                ABlueBullet* Bullet = GetWorld()->SpawnActor<ABlueBullet>(BulletClass, Loc, FRotator::ZeroRotator);
                if (Bullet) Bullet->Launch();
            }
        
        GetWorldTimerManager().SetTimer(ShootCooldownTimer, this, &APlayerSpaceship:: OnShootCooldownTimerTimeout, 1.0f, false, ShootCooldownInSec);
    }
}

void APlayerSpaceship::OnShootCooldownTimerTimeout(){
    CanShoot = true;
    
}
