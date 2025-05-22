#include "StellarDustGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

AActor* AStellarDustGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{

    TArray<AActor*> PlayerStarts;
    UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);

    if (PlayerStarts.Num() > 0)
    {
        return PlayerStarts[0];
    }

    return Super::ChoosePlayerStart_Implementation(Player);
}
