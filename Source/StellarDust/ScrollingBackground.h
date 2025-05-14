// ScrollingBackground.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperSpriteComponent.h"
#include "ScrollingBackground.generated.h"

UCLASS()
class STELLARDUST_API AScrollingBackground : public AActor
{
    GENERATED_BODY()

public:
    AScrollingBackground();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Scrolling")
    float ScrollSpeed = 200.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Scrolling")
    float TileLength = 1536.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Scrolling")
    UPaperSpriteComponent* SpriteA;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Scrolling")
    UPaperSpriteComponent* SpriteB;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Scrolling")
    UPaperSpriteComponent* SpriteC;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

private:
    void ScrollTiles(float DeltaSeconds);
};
