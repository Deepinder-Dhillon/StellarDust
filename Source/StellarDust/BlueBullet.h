// BlueBullet.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/TimerHandle.h"
#include "BlueBullet.generated.h"

UCLASS()
class STELLARDUST_API ABlueBullet : public AActor
{
    GENERATED_BODY()

public:
    ABlueBullet();

    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    USphereComponent* SphereComp;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UPaperFlipbookComponent* BulletFlipbook;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UPaperFlipbook* HitFlipbook;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UProjectileMovementComponent* ProjectileComp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MovementSpeed = 300.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D MovementDirection = FVector2D(0.f, 1.f);

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool IsLaunched = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool IsDisabled = false;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DestroyDelay = 2.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float HitDuration = 0.5f;

    FTimerHandle DeleteTimer;
    FTimerHandle HitTimer;
    
    void Launch();
    void DisableBullet();

    void OnDeleteTimerTimeout();
    void OnHitDestroy();

    UFUNCTION()
    void OverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                      int32 OtherBodyIndex, bool bFromSweep,const FHitResult& SweepResult);
};
