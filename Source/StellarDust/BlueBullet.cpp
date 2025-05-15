#include "BlueBullet.h"
#include "Enemy.h"

ABlueBullet::ABlueBullet()
{
        PrimaryActorTick.bCanEverTick = true;
        
        SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
        SetRootComponent(SphereComp);
        
        BulletSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BulletSprite"));
        BulletSprite->SetupAttachment(RootComponent);
        
        MovementDirection = FVector2D(0.0f, 10.0f);
    
        
    

}

void ABlueBullet::BeginPlay()
{
	Super::BeginPlay();
    if (AActor* O = GetOwner())
        {
            SphereComp->IgnoreActorWhenMoving(O, true);
        }

        SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ABlueBullet::OverlapBegin);
	
}

void ABlueBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
    if(IsLaunched){
        
        FVector2D DistanceToMove = MovementDirection * MovementSpeed * DeltaTime;
        FVector CurrentLocation = GetActorLocation();
        FVector NewLocation = CurrentLocation + FVector(0.0f, 0.0f, DistanceToMove.Y);
        
        SetActorLocation(NewLocation, true);
    }

}

void ABlueBullet::Launch(){
    if(IsLaunched) return;
        IsLaunched = true;
    float DeleteTime = 2.0f;
    GetWorldTimerManager().SetTimer(DeleteTimer, this, &ABlueBullet::OnDeleteTimerTimeout, 1.0f, false, DeleteTime);
    
}

void ABlueBullet::OnDeleteTimerTimeout(){
    Destroy();
    
}

void ABlueBullet::DisableBullet(){
    if(IsDisabled) return;
    IsDisabled = true;
    
    
    SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    BulletSprite->DestroyComponent();
    
}

void ABlueBullet::OverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                    bool bFromSweep,const FHitResult& SweepResult){
    AEnemy *Enemy = Cast<AEnemy>(OtherActor);
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan,
        FString::Printf(TEXT("Overlapped: %s"), *GetNameSafe(OtherActor))
    );
    
    if (Enemy){
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("overlap enemy"));
        DisableBullet();
        //Enemy->Die();
    }
}



