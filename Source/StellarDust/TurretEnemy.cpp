#include "TurretEnemy.h"
#include "Engine/GameEngine.h"

ATurretEnemy::ATurretEnemy(){
    PrimaryActorTick.bCanEverTick = true;

    RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
    SetRootComponent(RootScene);
    
    SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    SphereComp->SetupAttachment(RootScene);
    
    EnemyFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("EnemyFlipbook"));
    EnemyFlipbook->SetupAttachment(SphereComp);

    LeftSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("LeftSpawn"));
    LeftSpawn->SetupAttachment(EnemyFlipbook);

    RightSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("RightSpawn"));
    RightSpawn->SetupAttachment(EnemyFlipbook);
}

void ATurretEnemy::BeginPlay() {
    Super::BeginPlay();

    if (!Player) {
        AActor* Pawn = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerSpaceship::StaticClass());
        Player = Cast<APlayerSpaceship>(Pawn);
    }
    
    if (Player) {
            CanFollow = true;
        }
    
    GetWorldTimerManager().SetTimer(TurretStartTimer, this, &ATurretEnemy::EnterTurretMode, TurretModeInterval, false);
}

void ATurretEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (!IsAlive) return;

    FVector CurrentLocation = GetActorLocation();
    Target.Yaw = 0.0f;

    if (bInTurretMode && Player) {
        FVector CurrentLocation = GetActorLocation();
        FVector PlayerLocation = Player->GetActorLocation();
        
        FVector Start = FVector(CurrentLocation.X, CurrentLocation.Y, 0.0f);
        FVector End = FVector(PlayerLocation.X, PlayerLocation.Y, 0.0f);
        
        FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(Start, End);
        Rotation.Yaw += 90.0f;
        Target = Rotation;
    }
    
    FRotator CurrentRotation = RootScene->GetComponentRotation();
    FRotator NewRotation = UKismetMathLibrary::RInterpTo(CurrentRotation, Target, DeltaTime, RotationSpeed);
    
    RootScene->SetRelativeRotation(NewRotation);
    
    if (!bInTurretMode && !CanFollow) {
        const float Tolerance = 0.5f;
        if (FMath::IsNearlyZero(NewRotation.Yaw, Tolerance)){
            CanFollow = true;
        }
        }

    if (CanFollow && Player) {
        CurrentLocation.Y -= VerticalSpeed * DeltaTime;
        float DirX = FMath::Sign(Player->GetActorLocation().X - CurrentLocation.X);
        CurrentLocation.X += DirX * HorizontalSpeed * DeltaTime;
        
        SetActorLocation(CurrentLocation);
    }

    if (Player && FVector::Dist(CurrentLocation, Player->GetActorLocation()) < StopDistance) {
        Explosion();
    }
}

void ATurretEnemy::Hit() {
    --Health;
    if (Health <= 0) {
        Die();
    }
}

void ATurretEnemy::Explosion() {
    if (!IsAlive) return;
    
    IsAlive = false;
    CanFollow = false;
    SetActorEnableCollision(false);

    SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    EnemyFlipbook->SetTranslucentSortPriority(-5);
    EnemyFlipbook->SetFlipbook(ExplosionFlipbook);
    EnemyFlipbook->Play();
    
    GetWorldTimerManager().SetTimer(ExplosionTimer, this, &ATurretEnemy::OnDestroyExplosion, ExplosionDuration, false);
}

void ATurretEnemy::OnDestroyExplosion() {
    GetWorldTimerManager().ClearTimer(ExplosionTimer);
    Destroy();
}

void ATurretEnemy::Die() {
    if (!IsAlive) return;

    IsAlive = false;
    CanFollow = false;

    GetWorldTimerManager().ClearTimer(TurretStartTimer);
    GetWorldTimerManager().ClearTimer(TurretModeTimer);
    GetWorldTimerManager().ClearTimer(TurretShootCooldownTimer);

    if (ExplosionFlipbook) {
        EnemyFlipbook->SetFlipbook(ExplosionFlipbook);
        EnemyFlipbook->Play();
    }

    GetWorldTimerManager().SetTimer(DestroyTimer, this, &ATurretEnemy::OnDestroyTimerTimeout, DeathDelay, false);
}

void ATurretEnemy::OnDestroyTimerTimeout() {
    GetWorldTimerManager().ClearTimer(DestroyTimer);
    Destroy();
}

void ATurretEnemy::EnterTurretMode() {
    if (!IsAlive) return;

    bInTurretMode = true;
    BulletsShotThisCycle = 0;
    ++CurrentCycle;
    CanFollow = false;

    ShootTurretBullet();

    GetWorldTimerManager().SetTimer(TurretModeTimer, this, &ATurretEnemy::ExitTurretMode, TurretModeDuration, false);
}

void ATurretEnemy::ExitTurretMode() {
    bInTurretMode = false;

    if (CurrentCycle < TurretCycles) {
        GetWorldTimerManager().SetTimer(TurretStartTimer, this, &ATurretEnemy::EnterTurretMode, TurretModeInterval, false);
    }
}

void ATurretEnemy::ShootTurretBullet() {
    if (!IsAlive || !bInTurretMode || BulletsShotThisCycle >= BulletsPerCycle) return;

    FVector L = LeftSpawn->GetComponentLocation();
    FVector R = RightSpawn->GetComponentLocation();
    AEnemyBullet* LeftBullet  = GetWorld()->SpawnActor<AEnemyBullet>(BulletClass, L, FRotator::ZeroRotator);
    AEnemyBullet* RightBullet = GetWorld()->SpawnActor<AEnemyBullet>(BulletClass, R, FRotator::ZeroRotator);

    FRotator TurretRotation = RootScene->GetComponentRotation();
    TurretRotation += FRotator(0.f, -90.f, 0.f);
    
    FVector Forward3D = UKismetMathLibrary::GetForwardVector(TurretRotation);
    FVector2D BulletDirection = FVector2D(Forward3D.X, Forward3D.Y).GetSafeNormal();

    if (LeftBullet) {
        LeftBullet->EnemyBulletDirection = BulletDirection;
        LeftBullet->Launch();
    }
    if (RightBullet) {
        RightBullet->EnemyBulletDirection = BulletDirection;
        RightBullet->Launch();
    }

    ++BulletsShotThisCycle;
    GetWorldTimerManager().SetTimer(TurretShootCooldownTimer, this, &ATurretEnemy::OnTurretShootCooldown, BulletCooldown, false);
}

void ATurretEnemy::OnTurretShootCooldown() {
    ShootTurretBullet();
}
