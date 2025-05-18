#include "Enemy.h"
#include "Kismet/GameplayStatics.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
    
    SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    SetRootComponent(SphereComp);
    
    EnemyFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("EnemyFlipbook"));
    EnemyFlipbook->SetupAttachment(RootComponent);
    
    CenterSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("CenterSpawn"));
    CenterSpawn->SetupAttachment(EnemyFlipbook);
    
    LeftSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("LeftSpawn"));
    LeftSpawn->SetupAttachment(EnemyFlipbook);
    
    RightSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("RightSpawn"));
    RightSpawn->SetupAttachment(EnemyFlipbook);

}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
    if(!Player){
        AActor *PlayerActor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerSpaceship::StaticClass());
        if(PlayerActor){
            Player = Cast<APlayerSpaceship>(PlayerActor);
            CanFollow = true;
        }
    }
    SetupShootTimer();
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
    if(IsAlive && Player && CanFollow){
        FVector CurrentLocation = GetActorLocation();
        FVector PlayerLocation = Player->GetActorLocation();
        
        FVector DirectionToPlayer = PlayerLocation - CurrentLocation;
        float DistanceToPlayer = DirectionToPlayer.Size();
        
        if(DistanceToPlayer > StopDistance){
            DirectionToPlayer.Normalize();
            FVector NewLocation = CurrentLocation;
            
            if(CurrentLocation.Z > PlayerLocation.Z){
                float Xoffset = DirectionToPlayer.X * HorizontalSpeed * DeltaTime;
                NewLocation.X += Xoffset;
            }
        
            float Zoffset = -VerticalSpeed * DeltaTime;
            
            NewLocation.Z += Zoffset;
            SetActorLocation(NewLocation);
        }
        else {
            Explosion();
        }
    }
}

void AEnemy::Explosion(){
    if(!IsAlive) return;
    
    IsAlive = false;
    CanFollow= false;
    SetActorEnableCollision(false);
    
    SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    EnemyFlipbook->SetTranslucentSortPriority(-5);
    EnemyFlipbook->SetFlipbook(ExplosionFlipbook);
    EnemyFlipbook->Play();
    
    GetWorldTimerManager().SetTimer(ExplosionTimer, this, &AEnemy::OnDestoryExplosion, ExplosionDuration, false);
}

void AEnemy::OnDestoryExplosion(){
    GetWorldTimerManager().ClearTimer(ExplosionTimer);
    GetWorldTimerManager().ClearTimer(ShootTimer);
    GetWorldTimerManager().ClearTimer(ShootCooldownTimer);
    Destroy();
}

void AEnemy::Hit(){
    if (Health < 0){
        Explosion();
    }
    else{
        Health--;
    }
}

void AEnemy::SetupShootTimer(){
    if (IsAlive) {
        float RandomTime = FMath::RandRange(MinShootInterval, MaxShootInterval);
        GetWorldTimerManager().SetTimer(ShootTimer, this, &AEnemy::ShootAtPlayer,
                                        RandomTime, false);
    }
}

void AEnemy::ShootAtPlayer()
{
    if (IsAlive && CanShoot && BulletClass && GetWorld()) {
        CanShoot = false;

        if (BulletSpawn == 1) {
            FVector SpawnLocation = CenterSpawn->GetComponentLocation();
            AEnemyBullet* Bullet = GetWorld()->SpawnActor<AEnemyBullet>(BulletClass, SpawnLocation, FRotator::ZeroRotator);
            if (Bullet) {
                Bullet->Launch();
            }
        }
        else {
            FVector LeftSpawnLocation = LeftSpawn->GetComponentLocation();
            FVector RightSpawnLocation = RightSpawn->GetComponentLocation();
            
            AEnemyBullet* LeftBullet = GetWorld()->SpawnActor<AEnemyBullet>(BulletClass, LeftSpawnLocation,
                                                                            FRotator::ZeroRotator);
            AEnemyBullet* RightBullet = GetWorld()->SpawnActor<AEnemyBullet>(BulletClass, RightSpawnLocation,
                                                                             FRotator::ZeroRotator);
            
            if (LeftBullet) LeftBullet->Launch();
            if (RightBullet) RightBullet->Launch();
        }
        GetWorldTimerManager().SetTimer(ShootCooldownTimer, this, &AEnemy::OnShootCooldownTimerTimeout,
                                        ShootCooldownInSec, false);
    }
    SetupShootTimer();
}

void AEnemy::OnShootCooldownTimerTimeout()
{
    CanShoot = true;
}
