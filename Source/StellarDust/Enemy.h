#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "PaperSpriteComponent.h"
#include "Engine/TimerHandle.h"
#include "PlayerSpaceship.h"
#include "PaperFlipbookComponent.h"
#include "EnemyBullet.h"
#include "Enemy.generated.h"

UCLASS()
class STELLARDUST_API AEnemy : public AActor
{
	GENERATED_BODY()
	
public:
	AEnemy();
    
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    USphereComponent* SphereComp;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UPaperFlipbookComponent* EnemyFlipbook;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    USceneComponent* CenterSpawn;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    USceneComponent* LeftSpawn;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    USceneComponent* RightSpawn;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    APlayerSpaceship* Player;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UPaperFlipbook* ExplosionFlipbook;
    
    UPROPERTY(EditAnywhere)
    int32 Health = 5;
    
    UPROPERTY(EditAnywhere)
    int32 BulletSpawn = 1;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AEnemyBullet> BulletClass;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool IsAlive = true;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool CanFollow = false;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float VerticalSpeed = 10.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float HorizontalSpeed = 10.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StopDistance = 20.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DeathDelay = 2.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExplosionDuration = 0.5f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
    float MinShootInterval = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
    float MaxShootInterval = 3.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
    float ShootCooldownInSec = 0.5f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shooting")
    bool CanShoot = true;
   
    FTimerHandle DestoryTimer;
    FTimerHandle ExplosionTimer;
    FTimerHandle ShootTimer;
    FTimerHandle ShootCooldownTimer;
    
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
    void Die();
    void OnDestroyTimerTimeout();
    void Hit();
    void Explosion();
    void OnDestoryExplosion();
    void SetupShootTimer();
    void ShootAtPlayer();
    void OnShootCooldownTimerTimeout();

};
