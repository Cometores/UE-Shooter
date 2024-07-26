// Shoot Them Up Game.


#include "Components/STUCharacterMovementComponent.h"

#include "STUBaseCharacter.h"

// В зависимости от нажатого Shift увеличиваем скорость бега
float USTUCharacterMovementComponent::GetMaxSpeed() const
{
    const ASTUBaseCharacter* Player = Cast<ASTUBaseCharacter>(GetPawnOwner());
    if (Player && Player->IsRunning())
    {
        return Super::GetMaxSpeed() * RunModifier;
    }
    return Super::GetMaxSpeed();
}