#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/CapsuleComponent.h"
#include "PaperSpriteComponent.h"

#include "Components/SceneComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/Controller.h"
#include "Engine/TimerHandle.h"
#include "BlueBullet.h"
#include "PlayerSpaceship.generated.h"

UCLASS()
class STELLARDUST_API APlayerSpaceship : public APawn
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UCapsuleComponent* CapsuleComp;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UPaperSpriteComponent* PlayerSprite;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerSpaceship")
    UInputMappingContext* IMC_Spaceship;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayerSpaceship")
    int32 UpgradeLevel = 1;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="PlayerSpaceship")
    USceneComponent* CenterSpawn;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="PlayerSpaceship")
    USceneComponent* LeftSpawn;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="PlayerSpaceship")
    USceneComponent* RightSpawn;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayerSpaceship")
    float MovementSpeed = 1500.f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayerSpaceship")
    FVector2D HorizontalLimits;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayerSpaceship")
    FVector2D VerticalLimits;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayerSpaceship")
    TSubclassOf<ABlueBullet> BulletClass;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayerSpaceship")
    float ShootCooldownInSec = 0.3f;
    
    UPROPERTY(BlueprintReadWrite)
    FVector2D MovementDirection;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool CanShoot = true;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UInputAction* IA_Fire;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UInputAction* IA_Move;
    
    FTimerHandle ShootCooldownTimer;
    
    APlayerSpaceship();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    
    void MoveTriggered(const FInputActionValue& Value);
    void MoveCompleted(const FInputActionValue& Value);
    void Shoot(const FInputActionValue& Value);
    bool IsInMapBoundsHorizontal(float NewX);
    bool IsInMapBoundsVertical(float NewZ);
    void OnShootCooldownTimerTimeout();

};
