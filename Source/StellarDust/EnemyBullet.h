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
    UPaperSpriteComponent* EnemyBulletSprite;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EnemyBulletSpeed = 300.0f;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    APlayerSpaceship* Player;
    
    UPROPERTY(BlueprintReadWrite)
    FVector2D EnemyBulletDirection;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool IsLaunched = true;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool IsDisabled = false;
    
    FTimerHandle DeleteTimer;
    
	AEnemyBullet();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
    
    void Launch();
    void DisableBullet();
    void OnDeleteTimerTimeout();
    
    UFUNCTION()
    void OverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                      bool bFromSweep,const FHitResult& SweepResult);

};
