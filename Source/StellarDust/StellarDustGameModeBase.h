#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "StellarDustGameModeBase.generated.h"

UCLASS()
class STELLARDUST_API AStellarDustGameModeBase : public AGameModeBase
{
    GENERATED_BODY()
    
protected:
    virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
};
