// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

private:
	UPROPERTY(EditDefaultsOnly,Category=Montage)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere,Category=Sound)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere,Category=Particle)
	UParticleSystem* HitPartiacle;

	UPROPERTY(VisibleAnywhere)
	class UAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere)
	class UHealthBarComponent* HealthBarWidget;

	


public:	

};
