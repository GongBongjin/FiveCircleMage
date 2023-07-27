#include "Weapons/Magic/C_MagicBeam.h"

AC_MagicBeam::AC_MagicBeam()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AC_MagicBeam::BeginPlay()
{
	Collision->SetCapsuleHalfHeight(850.0f);
	Collision->SetCapsuleRadius(170.0f);

	Collision->OnComponentBeginOverlap.AddDynamic(this, &AC_MagicBeam::OnBeginOverlap);
}

void AC_MagicBeam::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsOtherActor(OtherActor) == false) return;

	DamageComp->GiveDmg(OtherActor, Damage, State);
}

void AC_MagicBeam::Tick(float DeltaTime)
{
	if (bActive == false) return;
	if (Duration < 0.0f)
	{
		bActive = false;
		ActiveCollision(false);

		return;
	}

	Super::Tick(DeltaTime);
}

void AC_MagicBeam::BeginCasting(FVector CasterPosition, FVector TargetPosition, FRotator Rotation)
{
	bActive = true;
	Duration = OriginDuration;

	SetCastingRotation(Rotation);

	SetActorLocation(CasterPosition);

	PlayParticle(MAIN_PARTICLE);
	PlayParticle(SUB_PARTICLE);

	ActiveCollision(true);
}

void AC_MagicBeam::PlayParticle(int32 ParticleType)
{
	if (ParticleType == MAIN_PARTICLE)
		MainParticleComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MainParticle, GetActorLocation());
	else if (ParticleType == END_PARTICLE && EndParticle != nullptr)
		EndParticleComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), EndParticle, GetActorLocation());
	else if (ParticleType == SUB_PARTICLE && SubParticle != nullptr)
		SubParticleComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SubParticle, GetActorLocation() + OwnerActor->GetActorForwardVector() * 170.0f);
}