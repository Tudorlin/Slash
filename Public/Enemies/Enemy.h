// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/HitInterface.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class SLASH_API AEnemy : public ACharacter , public IHitInterface   //继承接口，有了接口就不需要频繁的进行转换了，此处用于判断武器击中的物体
{
	GENERATED_BODY()

public:
	AEnemy();

protected:
	virtual void BeginPlay() override;
	virtual void GetHit() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
