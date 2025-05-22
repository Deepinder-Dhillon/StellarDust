#include "ScrollingBackground.h"

AScrollingBackground::AScrollingBackground() {
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

void AScrollingBackground::BeginPlay() {
    Super::BeginPlay();

    SpriteA->SetRelativeLocation(FVector(0, 0, 0));
    SpriteB->SetRelativeLocation(FVector(0, TileLength, 0));
    SpriteC->SetRelativeLocation(FVector(0, 2*TileLength, 0));
}

void AScrollingBackground::Tick(float DeltaSeconds) {
    Super::Tick(DeltaSeconds);
    ScrollTiles(DeltaSeconds);
}

void AScrollingBackground::ScrollTiles(float DeltaSeconds) {
    if (!SpriteA || !SpriteB || !SpriteC) return;

    float Step = -ScrollSpeed * DeltaSeconds;

    FVector A = SpriteA->GetRelativeLocation(); A.Y += Step;
    FVector B = SpriteB->GetRelativeLocation(); B.Y += Step;
    FVector C = SpriteC->GetRelativeLocation(); C.Y += Step;

    if (A.Y <= -TileLength) A.Y += 3*TileLength;
    if (B.Y <= -TileLength) B.Y += 3*TileLength;
    if (C.Y <= -TileLength) C.Y += 3*TileLength;

    SpriteA->SetRelativeLocation(A);
    SpriteB->SetRelativeLocation(B);
    SpriteC->SetRelativeLocation(C);
}
