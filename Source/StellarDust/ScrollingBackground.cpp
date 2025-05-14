// ScrollingBackground.cpp
#include "ScrollingBackground.h"

AScrollingBackground::AScrollingBackground()
{
    PrimaryActorTick.bCanEverTick = true;

    USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);

    SpriteA = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteA"));
    SpriteA->SetupAttachment(Root);

    SpriteB = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteB"));
    SpriteB->SetupAttachment(Root);

    SpriteC = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteC"));
    SpriteC->SetupAttachment(Root);
}

void AScrollingBackground::BeginPlay()
{
    Super::BeginPlay();

    SpriteA->SetRelativeLocation(FVector(0, 0, 0));
    SpriteB->SetRelativeLocation(FVector(0, 0, TileLength));
    SpriteC->SetRelativeLocation(FVector(0, 0, 2*TileLength));
}

void AScrollingBackground::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    ScrollTiles(DeltaSeconds);
}

void AScrollingBackground::ScrollTiles(float DeltaSeconds)
{
    if (!SpriteA || !SpriteB || !SpriteC) return;

    float Step = -ScrollSpeed * DeltaSeconds;

    FVector A = SpriteA->GetRelativeLocation(); A.Z += Step;
    FVector B = SpriteB->GetRelativeLocation(); B.Z += Step;
    FVector C = SpriteC->GetRelativeLocation(); C.Z += Step;

    if (A.Z <= -TileLength) A.Z += 3*TileLength;
    if (B.Z <= -TileLength) B.Z += 3*TileLength;
    if (C.Z <= -TileLength) C.Z += 3*TileLength;

    SpriteA->SetRelativeLocation(A);
    SpriteB->SetRelativeLocation(B);
    SpriteC->SetRelativeLocation(C);
}
