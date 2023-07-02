
#include "Enemies/Enemy.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);                //启用Mesh的重叠事件

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Ignore);

}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::PlayHitReactMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void AEnemy::GetHit(const FVector& HitPoint)
{
	const FVector ForwardVector = GetActorForwardVector();
	FVector HitVector = (FVector(HitPoint.X,HitPoint.Y,GetActorLocation().Z)-GetActorLocation()).GetSafeNormal();         //受击向量与向前向量处同一高度并规格化
	const double cosTheta = FVector::DotProduct(ForwardVector,HitVector);             //算出夹角的余弦
	double Theta = FMath::Acos(cosTheta);                      //算出夹角，弧度制
	Theta = FMath::RadiansToDegrees(Theta);        //夹角θ

	FVector CrossVector = FVector::CrossProduct(ForwardVector,HitVector);    //叉积
	if(CrossVector.Z<0.f)
	{
		Theta *= -1.f;
	}
	FName SectionName = FName("FromRight");
	if(-45.f<=Theta&&Theta<=45.f) SectionName = FName("FromFront");
	else if(135.f<=Theta||Theta<=-135.f) SectionName = FName("FromBack");
	else if(-135.f<Theta&&Theta-45.f) SectionName = FName("FromLeft");

	PlayHitReactMontage(SectionName);
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossVector * 100.f, 5.f, FColor::Blue, 5.f);
}

