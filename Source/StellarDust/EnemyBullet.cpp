#include "EnemyBullet.h"
#include "Kismet/GameplayStatics.h"

AEnemyBullet::AEnemyBullet()
{
	PrimaryActorTick.bCanEverTick = true;
    
    SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    SetRootComponent(SphereComp);
    
    EnemyBulletSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("EnemyBulletSprite"));
    EnemyBulletSprite->SetupAttachment(RootComponent);
    
    EnemyBulletDirection = FVector2D(0.0f, 5.0f);
}
void AEnemyBullet::BeginPlay()
{
	Super::BeginPlay();
    
    SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBullet::OverlapBegin);
    
    
}

void AEnemyBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

//    if(IsLaunched){
        FVector2D DistanceToMove = EnemyBulletDirection * EnemyBulletSpeed * DeltaTime;
        FVector CurrentLocation = GetActorLocation();
        FVector NewLocation = CurrentLocation + FVector(0.0f, 0.0f, -DistanceToMove.Y);
        
        SetActorLocation(NewLocation, true);
//    }

}

void AEnemyBullet::Launch(){
    if(IsLaunched) return;
    
    IsLaunched = true;
    float DeleteTime = 2.0f;
    GetWorldTimerManager().SetTimer(DeleteTimer, this, &AEnemyBullet::OnDeleteTimerTimeout, 1.0f, false, DeleteTime);
}

void AEnemyBullet::OnDeleteTimerTimeout(){
    Destroy();
}

void AEnemyBullet::DisableBullet(){
    if(IsDisabled) return;
    
    IsDisabled = true;
    SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    EnemyBulletSprite->DestroyComponent();
}

void AEnemyBullet::OverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                    bool bFromSweep,const FHitResult& SweepResult){
    AActor *PlayerActor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerSpaceship::StaticClass());
    Player = Cast<APlayerSpaceship>(PlayerActor);
    
    if (Player){
        DisableBullet();
//        Enemy->Hit();
    }
}

