// PlayerSpaceship.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/CapsuleComponent.h"
#include "PaperSpriteComponent.h"

#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/Controller.h"
#include "PlayerSpaceship.generated.h"

UCLASS()
class STELLARDUST_API APlayerSpaceship : public APawn
{
	GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UCapsuleComponent* CapsuleComp;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UPaperSpriteComponent* PlayerSprite;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UInputMappingContext* IMC_Spaceship;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UInputAction* IA_Fire;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UInputAction* IA_Move;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
    float MovementSpeed = 500.f;
    
	APlayerSpaceship();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    void MoveTriggered(const FInputActionValue& Value);
    void Shoot(const FInputActionValue& Value);

};
