// Shoot Them Up Game.


#include "Dev/STUDevDamageActor.h"

#include "Kismet/GameplayStatics.h"

ASTUDevDamageActor::ASTUDevDamageActor()
{
    PrimaryActorTick.bCanEverTick = true;

    SceneComponent = CreateDefaultSubobject<USceneComponent>("Scene Component");
    SetRootComponent(SceneComponent);
}

void ASTUDevDamageActor::BeginPlay()
{
    Super::BeginPlay();
}

// Applying Damage every Tick
void ASTUDevDamageActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 24, SphereColor);
    UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), Radius, DamageType, {}, this, nullptr, DoFullDamage);
}