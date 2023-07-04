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

private:
	UPROPERTY(VisibleAnywhere)
	class UGeometryCollectionComponent* GeometryCollection;

public:	


};
