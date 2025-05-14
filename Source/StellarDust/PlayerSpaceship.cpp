// PlayerSpaceship.cpp
#include "PlayerSpaceship.h"

APlayerSpaceship::APlayerSpaceship()
{
	PrimaryActorTick.bCanEverTick = true;
    
    CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
    SetRootComponent(CapsuleComp);

    PlayerSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PlayerSprite"));
    PlayerSprite->SetupAttachment(RootComponent);

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
    


}

void APlayerSpaceship::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    
    if(EnhancedInputComponent){
        EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APlayerSpaceship::MoveTriggered);
        
        EnhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Started, this, &APlayerSpaceship::Shoot);
        EnhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Triggered, this, &APlayerSpaceship::Shoot);
        }

}

void APlayerSpaceship::MoveTriggered(const FInputActionValue& Value){
    
    const FVector2D DeltaXY = Value.Get<FVector2D>();
        if (!DeltaXY.IsNearlyZero())
        {

            FVector Movement = FVector(DeltaXY.X, 0.f, DeltaXY.Y)
                            * MovementSpeed
                            * GetWorld()->GetDeltaSeconds();

            AddActorWorldOffset(Movement, true);
        }
    
}

void APlayerSpaceship::Shoot(const FInputActionValue& Value){
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Shoot"));
    
}

