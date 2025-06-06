#include "EnemyBullet.h"
#include "Kismet/GameplayStatics.h"

AEnemyBullet::AEnemyBullet()
{
	PrimaryActorTick.bCanEverTick = true;
    
    SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    SetRootComponent(SphereComp);
    
    EnemyBulletFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("EnemyBulletFlipbook"));
    EnemyBulletFlipbook->SetupAttachment(RootComponent);
    
}

void AEnemyBullet::BeginPlay() {
	Super::BeginPlay();
    
    SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBullet::OverlapBegin);
    if (!Player){
        AActor *PlayerActor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerSpaceship::StaticClass());
        Player = Cast<APlayerSpaceship>(PlayerActor);
    }
}

void AEnemyBullet::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    if (IsLaunched) {
        FVector2D Delta2D = EnemyBulletDirection * EnemyBulletSpeed * DeltaTime;
        FVector   NewLoc = GetActorLocation() + FVector(Delta2D, 0.f);
        SetActorLocation(NewLoc, true);
    }
}


void AEnemyBullet::Launch(){
    if(IsLaunched) return;
    
    IsLaunched = true;
    GetWorldTimerManager().SetTimer(DeleteTimer, this, &AEnemyBullet::OnDeleteTimerTimeout, DeleteTime, false);
}

void AEnemyBullet::OnDeleteTimerTimeout(){
    Destroy();
}

void AEnemyBullet::DisableBullet(){
    if(IsDisabled) return;
    
    IsDisabled = true;
    EnemyBulletSpeed = 0.0;
    SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    EnemyBulletFlipbook->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.4f));
    EnemyBulletFlipbook->SetFlipbook(EnemyHitFlipbook);
    EnemyBulletFlipbook->Play();
    
    GetWorldTimerManager().SetTimer(HitTimer, this, &AEnemyBullet::OnHitDestroy, HitDuration, false);
}

void AEnemyBullet::OnHitDestroy() {
    GetWorldTimerManager().ClearTimer(HitTimer);
    EnemyBulletFlipbook->DestroyComponent();
}

void AEnemyBullet::OverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                bool bFromSweep,const FHitResult& SweepResult) {
    
    APlayerSpaceship* HitPlayer = Cast<APlayerSpaceship>(OtherActor);
    if (HitPlayer){
        DisableBullet();
    }
    
}

