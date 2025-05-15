// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "PaperSpriteComponent.h"
#include "Engine/TimerHandle.h"
#include "PlayerSpaceship.h"
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
    UPaperSpriteComponent* EnemySprite;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    APlayerSpaceship* Player;
    
    UPROPERTY(EditAnywhere)
    int32 Health = 5;
    
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
    float FlickerInterval = 0.2f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DeathDelay = 2.0f;
//    
//    FTimerHandle FlickerTimer;
//    FTimerHandle DeathTimer;



	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
//    void BeginDeathSequence();
//    void Flicker();
//    void FinishDeath();
    
//    UFUNCTION()
//        void OnOverlapBegin(
//            UPrimitiveComponent* OverlappedComp,
//            AActor* OtherActor,
//            UPrimitiveComponent* OtherComp,
//            int32 OtherBodyIndex,
//            bool bFromSweep,
//            const FHitResult& SweepResult
//        );

};
