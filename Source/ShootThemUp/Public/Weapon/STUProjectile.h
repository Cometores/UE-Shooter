// Shoot Them Up Game.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUProjectile.generated.h"

class USTUWeaponFXComponent;
class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUProjectile : public AActor
{
    GENERATED_BODY()

public:
    ASTUProjectile();

    void SetShotDirection(const FVector& Direction) { ShotDirection = Direction; };

protected:
    UPROPERTY(VisibleAnywhere, Category="Weapon")
    USphereComponent* CollisionComponent;

    UPROPERTY(VisibleAnywhere, Category="Weapon")
    UProjectileMovementComponent* MovementComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Weapon")
    float DamageRadius = 200.0f;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Weapon")
    float DamageAmount = 50.0f;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Weapon")
    bool DoFullDamage = false;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Weapon")
    float LifeSeconds = 5.0f;
    
    UPROPERTY(VisibleAnywhere, Category="VFX")
    USTUWeaponFXComponent* WeaponFXComponent;
    
    UFUNCTION()
    virtual void BeginPlay() override;

private:
    FVector ShotDirection;

    UFUNCTION()
    void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        FVector NormalImpulse, const FHitResult& Hit);

    AController* GetController() const;
};