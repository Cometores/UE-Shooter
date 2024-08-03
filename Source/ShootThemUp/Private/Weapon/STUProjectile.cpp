// Shoot Them Up Game.


#include "Weapon/STUProjectile.h"

#include "Components/SphereComponent.h"

ASTUProjectile::ASTUProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("Sphere Component");
    CollisionComponent->InitSphereRadius(5.0);
    SetRootComponent(CollisionComponent);
}

void ASTUProjectile::BeginPlay()
{
    Super::BeginPlay();

}