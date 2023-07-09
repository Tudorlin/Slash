// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterTpes.h"
#include "Interface/HitInterface.h"
#include "GameFramework/Character.h"
#include "Sound/SoundCue.h"
#include "Enemy.generated.h"

UCLASS()
class SLASH_API AEnemy : public ACharacter , public IHitInterface   //继承接口，有了接口就不需要频繁的进行转换了，此处用于判断武器击中的物体
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	virtual void GetHit_Implementation(const FVector& HitPoint) override;;

	void PlayHitReactMontage(FName SectionName);
	void DirectionalHitReact(const FVector& HitPoint);

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(BlueprintReadOnly)
	EDeathPoss DeathPoss = EDeathPoss::EDP_Alive;

private:
	UPROPERTY(EditDefaultsOnly,Category=Montage)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly,Category=Montage)
	UAnimMontage* DeathMontage;
	

	UPROPERTY(EditAnywhere,Category=Sound)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere,Category=Particle)
	UParticleSystem* HitPartiacle;

	UPROPERTY(VisibleAnywhere,meta=(AllowPrivateAccess = "true"))
	class UAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere)
	class UHealthBarComponent* HealthBarWidget;

	UPROPERTY()
	AActor* CombatTarget = nullptr;

	UPROPERTY(EditAnywhere)
	float CombatRadius = 500.f;

	UPROPERTY(EditAnywhere)
	float PartolRadius = 200.f;

	/**
	 **导航网格
	 **/
	UPROPERTY()
	class AAIController* EnemyController;

	UPROPERTY(EditInstanceOnly,Category="AI Navigation")
	AActor* PartolTarget;    //当前的导航点

	UPROPERTY(EditInstanceOnly,Category="AI Navigation")
	TArray<AActor*> PartolTargets;   //导航网格体中包含的导航点

	bool InTargetRange(AActor* Target,float Radius);

public:
	void Die();
};
