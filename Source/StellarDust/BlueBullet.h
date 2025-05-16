// BlueBullet.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "PaperSpriteComponent.h"
#include "Engine/TimerHandle.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BlueBullet.generated.h"

UCLASS()
class STELLARDUST_API ABlueBullet : public AActor
{
	GENERATED_BODY()
	
public:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    USphereComponent* SphereComp;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UPaperSpriteComponent* BulletSprite;
    
//    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//    UProjectileMovementComponent* ProjectileComp;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MovementSpeed = 300.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D MovementDirection;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool IsLaunched = false;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool IsDisabled = false;
    
    FTimerHandle DeleteTimer;
    
	ABlueBullet();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
    
    void Launch();
    void DisableBullet();
    void OnDeleteTimerTimeout();
    
    UFUNCTION()
    void OverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                      bool bFromSweep,const FHitResult& SweepResult);
};
