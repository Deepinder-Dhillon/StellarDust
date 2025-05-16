#include "Enemy.h"
#include "Kismet/GameplayStatics.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
    
    SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    SetRootComponent(SphereComp);
    
    EnemyFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("EnemyFlipbook"));
    EnemyFlipbook->SetupAttachment(RootComponent);

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
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
    if(IsAlive && Player && CanFollow){
        FVector CurrentLocation = GetActorLocation();
        FVector PlayerLocation = Player->GetActorLocation();
        
        FVector DirectionToPlayer = PlayerLocation - CurrentLocation;
        float DistanceToPlayer = DirectionToPlayer.Size();
        
        
        if( DistanceToPlayer > StopDistance){
            DirectionToPlayer.Normalize();
            float Xoffset = DirectionToPlayer.X * HorizontalSpeed * DeltaTime;
            
            float Zoffset = -VerticalSpeed * DeltaTime;
            FVector NewLocation = CurrentLocation;
            NewLocation.X += Xoffset;
            NewLocation.Z += Zoffset;
            
            SetActorLocation(NewLocation);
        }
        else {
            Explosion();
        }
    }
}

void AEnemy::Die(){
    if(!IsAlive) return;
    
    IsAlive = false;
    CanFollow= false;
    SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    SetActorEnableCollision(false);
    EnemyFlipbook->SetTranslucentSortPriority(-5);
    GetWorldTimerManager().SetTimer(FlickerTimer, this, &AEnemy::Flicker, FlickerInterval, true);
    GetWorldTimerManager().SetTimer(DestoryTimer, this, &AEnemy::OnDestroyTimerTimeout, 1.0f, false, DeathDelay);
    
}

void AEnemy::OnDestroyTimerTimeout(){
    GetWorldTimerManager().ClearTimer(FlickerTimer);
    Destroy();
}

void AEnemy::Explosion(){
    if(!IsAlive) return;
    IsAlive = false;
    CanFollow= false;
    SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    SetActorEnableCollision(false);
    EnemyFlipbook->SetFlipbook(ExplosionFlipbook);
    EnemyFlipbook->Play();
    
    GetWorldTimerManager().SetTimer(ExplosionTimer, this, &AEnemy::OnDestoryExplosion, 1.0f, false, ExplosionDuration);
}

void AEnemy::OnDestoryExplosion(){
    GetWorldTimerManager().ClearTimer(ExplosionTimer);
    Destroy();
}

void AEnemy::Flicker(){
    Transparent = !Transparent;
    EnemyFlipbook->SetVisibility(!Transparent, true);
}

void AEnemy::Hit(){
    if (Health < 0){
        Die();
    }
    else{
        Health--;
    }
}

