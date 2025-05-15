
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
    
    SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    SetRootComponent(SphereComp);
    
    EnemySprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("EnemySprite"));
    EnemySprite->SetupAttachment(RootComponent);

}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
    if(!Player){
        AActor *PlayerActor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerSpaceship::StaticClass());
        if(PlayerActor){
            Player = Cast<APlayerSpaceship>(PlayerActor);
        }
    }
    
	
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
    if(IsAlive && Player){
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
        
        
    }

}

