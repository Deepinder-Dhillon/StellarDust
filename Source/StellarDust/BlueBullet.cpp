// BlueBullet.cpp
#include "BlueBullet.h"
#include "Enemy.h"
#include "TurretEnemy.h"
#include "Engine/World.h"
#include "TimerManager.h"

ABlueBullet::ABlueBullet(){
    PrimaryActorTick.bCanEverTick = false;

    SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    SetRootComponent(SphereComp);
    
    BulletFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("BulletFlipbook"));
    BulletFlipbook->SetupAttachment(RootComponent);

    ProjectileComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
    ProjectileComp->UpdatedComponent = SphereComp;
    ProjectileComp->InitialSpeed = MovementSpeed;
    ProjectileComp->MaxSpeed = MovementSpeed;
    ProjectileComp->ProjectileGravityScale = 0.f;
    ProjectileComp->bRotationFollowsVelocity = false;
    ProjectileComp->Velocity = FVector::ZeroVector;
}

void ABlueBullet::BeginPlay(){
    Super::BeginPlay();
    
    SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ABlueBullet::OverlapBegin);
}

void ABlueBullet::Launch(){
    if (IsLaunched)
        return;
    
    IsLaunched = true;
    
    ProjectileComp->Velocity = FVector(MovementDirection.X, MovementDirection.Y, 0.f) * MovementSpeed;
    GetWorldTimerManager().SetTimer(DeleteTimer,this, &ABlueBullet::OnDeleteTimerTimeout, DestroyDelay, false);
}

void ABlueBullet::DisableBullet(){
    if (IsDisabled)
        return;

    IsDisabled = true;
    ProjectileComp->StopMovementImmediately();
    ProjectileComp->Deactivate();
    SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    BulletFlipbook->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
    BulletFlipbook->SetTranslucentSortPriority(10);
    BulletFlipbook->SetFlipbook(HitFlipbook);
    BulletFlipbook->Play();
    
    
    GetWorldTimerManager().SetTimer(HitTimer, this, &ABlueBullet::OnHitDestroy, HitDuration, false);
}


void ABlueBullet::OnHitDestroy(){
    GetWorldTimerManager().ClearTimer(HitTimer);
    BulletFlipbook->DestroyComponent();
}

void ABlueBullet::OnDeleteTimerTimeout(){
    Destroy();
}

void ABlueBullet::OverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                               int32 OtherBodyIndex, bool bFromSweep,const FHitResult& SweepResult){
    AEnemy *Enemy = Cast<AEnemy>(OtherActor);
    ATurretEnemy *TurretEnemy = Cast<ATurretEnemy>(OtherActor);
    
    
    if (Enemy && Enemy->IsAlive) {
        DisableBullet();
        Enemy->Hit();
    }
    else if (TurretEnemy &&  TurretEnemy->IsAlive){
        DisableBullet();
        TurretEnemy->Hit();
    }
}
