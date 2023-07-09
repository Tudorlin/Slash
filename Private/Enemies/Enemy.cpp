
#include "Enemies/Enemy.h"

#include "AIController.h"
#include "Component/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include"Components/WidgetComponent.h"
#include"GameFramework/CharacterMovementComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);                //启用Mesh的重叠事件

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Ignore);

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attritube Component"));
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	if(HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}

	EnemyController = Cast<AAIController>(GetController());  //初始化AI控制器
	if(EnemyController&&PartolTarget)
	{
		FAIMoveRequest MoveRequest;    //用于设置目标半径等属性，这玩意是个结构体
		MoveRequest.SetGoalActor(PartolTarget);
		MoveRequest.SetAcceptanceRadius(15.f);
		FNavPathSharedPtr NavPath;
		EnemyController->MoveTo(MoveRequest,&NavPath);
		TArray<FNavPathPoint>& PathPoints = NavPath->GetPathPoints();  //获取网格中的导航点
		for(auto& Point : PathPoints)
		{
			const FVector& Location = Point.Location;
		}
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(CombatTarget)
	{
		if(!InTargetRange(CombatTarget,CombatRadius))
		{
			CombatTarget = nullptr;
			if(HealthBarWidget)
			{
				HealthBarWidget->SetVisibility(false);
			}
		}
	}
	if(PartolTarget&&EnemyController)
	{
		if(InTargetRange(PartolTarget,PartolRadius))
		{
			TArray<AActor*> Targets;             //防止在某一个导航点一直巡逻
			for(AActor* Target : PartolTargets)  
			{
				if(Target !=PartolTarget)    //如果当前的导航点不存在于PartolTargets中就添加进去，此数组存储为“有效”导航点
					Targets.AddUnique(Target);
			}
			const int32 TargetNums = Targets.Num();
			if(TargetNums > 0)           //有导航点被加入数组则执行
			{
				const int32 Section = FMath::RandRange(0,TargetNums-1);
				AActor* TargetPoint = Targets[Section];
				PartolTarget = TargetPoint;

				FAIMoveRequest MoveRequest;
				MoveRequest.SetGoalActor(PartolTarget);
				MoveRequest.SetAcceptanceRadius(15.f);
				EnemyController->MoveTo(MoveRequest);
			}
		}
	}
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

bool AEnemy::InTargetRange(AActor* Target, float Radius)
{
	const float Distance = (Target->GetActorLocation()-GetActorLocation()).Size();
	return Distance <= Radius;
}

void AEnemy::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);

		const int32 Selection = FMath::RandRange(0, 4);
		FName SectionName = FName();
		switch (Selection)
		{
		case 0:
			SectionName = FName("Death1");
			DeathPoss = EDeathPoss::EDP_Death1;
			break;
		case 1:
			SectionName = FName("Death2");
			DeathPoss = EDeathPoss::EDP_Death2;
			break;
		case 2:
			SectionName = FName("Death3");
			DeathPoss = EDeathPoss::EDP_Death3;
			break;
		case 3:
			SectionName = FName("Death4");
			DeathPoss = EDeathPoss::EDP_Death4;
			break;
		case 4:
			SectionName = FName("Death5");
			DeathPoss = EDeathPoss::EDP_Death5;
			break;
		default:
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);
	}
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(3.f);
}

void AEnemy::DirectionalHitReact(const FVector& HitPoint)
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

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if(AttributeComponent&&HealthBarWidget)
	{
		AttributeComponent->ReceiveDamage(DamageAmount);                            //接收伤害以及血量百分比写在属性组件中，由蓝图身上的属性组件控制
		HealthBarWidget->SetHealthPercent(AttributeComponent->GetHealthPercet());    //接收完伤害之后设置百分比
		
	}
	CombatTarget = EventInstigator->GetPawn();
	return DamageAmount;
}

void AEnemy::GetHit_Implementation(const FVector& HitPoint)
{
	if(HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}
	if(AttributeComponent&&AttributeComponent->IsAlive())
	{
		DirectionalHitReact(HitPoint);
	}
	else
	{
		Die();
	}
	if(HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			HitPoint);
	}
	if(GetWorld()&&HitPartiacle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitPartiacle,
			HitPoint);
	}
}

