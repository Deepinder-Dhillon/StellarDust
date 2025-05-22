#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "PaperFlipbookComponent.h"
#include "Engine/TimerHandle.h"
#include "EnemyBullet.h"
#include "PlayerSpaceship.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "TurretEnemy.generated.h"

UCLASS()
class STELLARDUST_API ATurretEnemy : public AActor
{
    GENERATED_BODY()
    
public:
    ATurretEnemy();

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    USceneComponent* RootScene;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    USphereComponent* SphereComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UPaperFlipbookComponent* EnemyFlipbook;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    USceneComponent* LeftSpawn;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    USceneComponent* RightSpawn;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    APlayerSpaceship* Player;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Turret")
    UPaperFlipbook* ExplosionFlipbook;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Turret")
    int32 Health  = 5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Turret")
    TSubclassOf<AEnemyBullet> BulletClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Turret")
    float VerticalSpeed = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Turret")
    float HorizontalSpeed = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Turret")
    float StopDistance = 20.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Turret")
    int32 TurretCycles = 3;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Turret")
    float TurretModeDuration = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Turret")
    float TurretModeInterval = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Turret")
    int32 BulletsPerCycle = 3;
 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Turret")
    float BulletCooldown = 0.2f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Turret")
    float RotationSpeed = 10.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Turret")
    float DeathDelay = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Turret")
    float ExplosionDuration = 0.5f;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool IsAlive = true;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool CanFollow = false;
    
    bool bInTurretMode = false;
    int32 CurrentCycle = 0;
    int32 BulletsShotThisCycle = 0;
    FRotator Target;
    
    FTimerHandle DestroyTimer;
    FTimerHandle ExplosionTimer;
    FTimerHandle TurretStartTimer;
    FTimerHandle TurretModeTimer;
    FTimerHandle TurretShootCooldownTimer;
    
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    void Die();
    void OnDestroyTimerTimeout();
    void Hit();
    void Explosion();
    void OnDestroyExplosion();
    void EnterTurretMode();
    void ExitTurretMode();
    void ShootTurretBullet();
    void OnTurretShootCooldown();
};
