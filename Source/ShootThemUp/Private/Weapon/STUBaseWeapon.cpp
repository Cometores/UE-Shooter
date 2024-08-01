// Shoot Them Up Game.


#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY_STATIC(BaseWeaponLog, All, All);

ASTUBaseWeapon::ASTUBaseWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);
}

void ASTUBaseWeapon::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponMesh);
}

void ASTUBaseWeapon::StartFire()
{
    MakeShot();
    GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTUBaseWeapon::MakeShot, TimeBetweenShots, true);
}

void ASTUBaseWeapon::StopFire()
{
    GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

void ASTUBaseWeapon::MakeShot()
{
    if (!GetWorld())
        return;

    // DrawTraceForGun();

    FVector TraceStart, TraceEnd;
    if(!GetTraceData(TraceStart, TraceEnd))
        return;

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    if(HitResult.bBlockingHit)
    {
        MakeDamage(HitResult);
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);
        
        UE_LOG(BaseWeaponLog, Display, TEXT("Bone: %s"), *HitResult.BoneName.ToString());
    }
}

void ASTUBaseWeapon::DrawTraceForGun()
{
    const FTransform SocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);
    const FVector Start = SocketTransform.GetLocation();
    const FVector Direction = SocketTransform.GetRotation().GetForwardVector();
    const FVector End = Start + Direction * TraceMaxDistance;

    FHitResult HitResult;
    GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);

    if(HitResult.bBlockingHit)
    {
        DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 3.0f, 0, 3.0f);
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);
    }
}


APlayerController* ASTUBaseWeapon::GetPlayerController() const
{
    const auto Player = Cast<ACharacter>(GetOwner());
    if(!Player)
        return nullptr;
    
    return Player->GetController<APlayerController>();
}

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
    const auto Controller = GetPlayerController();
    if (!Controller) return false;
    
    Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
    return true;
}

FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const
{
    return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

bool ASTUBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if(!GetPlayerViewPoint(ViewLocation, ViewRotation))
        return false;
    
    TraceStart = ViewLocation;

    // spreading the shots
    const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
    const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
    
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
    
    return true;
}

void ASTUBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const
{
    if(!GetWorld()) return;

    // игнорируем получение урона обладателем оружия
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    
    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionParams);
}

void ASTUBaseWeapon::MakeDamage(const FHitResult& HitResult)
{
    const auto DamagedActor = HitResult.GetActor();
    if(!DamagedActor) return;

    DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
}