// Shoot Them Up Game.


#include "Components/STUHealthComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)

USTUHealthComponent::USTUHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}


bool USTUHealthComponent::TryToAddHealth(float HealthAmount)
{
    if (IsDead() || IsHealthFull())
    return false;

    SetHealth(Health + HealthAmount);
    return true;
}

bool USTUHealthComponent::IsHealthFull() const
{
    return FMath::IsNearlyEqual(Health, MaxHealth);
}

void USTUHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    check(MaxHealth > 0);

    SetHealth(MaxHealth);

    AActor* ComponentOwner = GetOwner();
    if (ComponentOwner)
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamageHandle);
    
}


// Вызывается, когда игрок получает урон
void USTUHealthComponent::OnTakeAnyDamageHandle(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
    AController* InstigatedBy, AActor* DamageCauser)
{
    if (Damage <= 0.0f || IsDead() || !GetWorld())
        return;
    
    SetHealth(Health - Damage);

    if (IsDead())
    {
        OnDeath.Broadcast(); // Вызываем смерть персонажа
        GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
    }
    else if (AutoHeal)
    {
        // таймер на HealUpdate
        GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &USTUHealthComponent::HealUpdate, HealUpdateTime,
            true, HealDelay);
    }
}

// Автолечение
void USTUHealthComponent::HealUpdate()
{
    SetHealth(Health + HealModifier);

    if (IsHealthFull() && GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
    }
}

void USTUHealthComponent::SetHealth(float NewHealth)
{
    Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    OnHealthChanged.Broadcast(Health);
}