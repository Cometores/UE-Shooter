// Shoot Them Up Game.


#include "UI/STUPlayerHUDWidget.h"

#include "STUWeaponComponent.h"
#include "Components//STUHealthComponent.h"

float USTUPlayerHUDWidget::GetHealthPercent() const
{
    const auto HealthComponent = GetHealthComponent();
    if (!HealthComponent)
        return 0.0f;

    return HealthComponent->GetHealthPercent();
}

bool USTUPlayerHUDWidget::GetWeaponUIData(FWeaponUIData& UIData) const
{
    const auto WeaponComponent = GetWeaponComponent();
    if (!WeaponComponent)
        return false;

    return WeaponComponent->GetWeaponUIData(UIData);
}

bool USTUPlayerHUDWidget::GetWeaponAmmoData(FAmmoData& AmmoData) const
{
    const auto WeaponComponent = GetWeaponComponent();
    if (!WeaponComponent)
        return false;

    return WeaponComponent->GetWeaponAmmoData(AmmoData);
}

bool USTUPlayerHUDWidget::IsPlayerAlive() const
{
    const auto HealthComponent = GetHealthComponent();
    return HealthComponent && !HealthComponent->IsDead();
}

bool USTUPlayerHUDWidget::IsPlayerSpectating() const
{
    const auto Controller = GetOwningPlayer();
    return Controller && Controller->GetStateName() == NAME_Spectating;
}

USTUWeaponComponent* USTUPlayerHUDWidget::GetWeaponComponent() const
{
    const auto Player = GetOwningPlayerPawn();
    if (!Player)
        return nullptr;

    const auto Component = Player->GetComponentByClass(USTUWeaponComponent::StaticClass());
    return Cast<USTUWeaponComponent>(Component);
}

USTUHealthComponent* USTUPlayerHUDWidget::GetHealthComponent() const
{
    const auto Player = GetOwningPlayerPawn();
    if (!Player)
        return nullptr;

    const auto Component = Player->GetComponentByClass(USTUHealthComponent::StaticClass());
    return Cast<USTUHealthComponent>(Component);
}