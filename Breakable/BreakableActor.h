// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/HitInterface.h"
#include "BreakableActor.generated.h"

UCLASS()
class SLASH_API ABreakableActor : public AActor , public IHitInterface
{
	GENERATED_BODY()
	
public:	
	ABreakableActor();
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;
	
	virtual void GetHit_Implementation(const FVector& HitPoint) override;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	class UGeometryCollectionComponent* GeometryCollection;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	class UCapsuleComponent* Capsule;

private:

	UPROPERTY(EditAnywhere,Category=BreakableActor)
	TArray<TSubclassOf<class ATreasure>> TreasureClasses;    //需要生成的是蓝图类，不能直接使用Treasure,否则生成的是c++类

	bool Canbroken = false;
public:	


};
