#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "PaperSpriteComponent.h"
#include "Engine/TimerHandle.h"
#include "PlayerSpaceship.h"
#include "EnemyBullet.generated.h"

UCLASS()
class STELLARDUST_API AEnemyBullet : public AActor
{
	GENERATED_BODY()
	
public:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    USphereComponent* SphereComp;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UPaperFlipbookComponent* EnemyBulletFlipbook;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Bullet")
    UPaperFlipbook* EnemyHitFlipbook;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Bullet")
    float EnemyBulletSpeed = 300.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Bullet")
    float DeleteTime = 2.0f;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Bullet")
    APlayerSpaceship* Player;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Bullet")
    FVector2D EnemyBulletDirection = FVector2D(0.f, -1.0f);
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Bullet")
    float HitDuration = 0.5f;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool IsLaunched = true;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool IsDisabled = false;
    
    FTimerHandle DeleteTimer;
    FTimerHandle HitTimer;
    
	AEnemyBullet();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
    
    void Launch();
    void DisableBullet();
    void OnDeleteTimerTimeout();
    void OnHitDestroy();
    
    UFUNCTION()
    void OverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                      int32 OtherBodyIndex, bool bFromSweep,const FHitResult& SweepResult);

};
