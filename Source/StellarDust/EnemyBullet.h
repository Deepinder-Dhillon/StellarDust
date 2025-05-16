#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBullet.generated.h"

UCLASS()
class STELLARDUST_API AEnemyBullet : public AActor
{
	GENERATED_BODY()
	
public:
	AEnemyBullet();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
