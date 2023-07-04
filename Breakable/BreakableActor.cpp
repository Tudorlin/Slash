// Fill out your copyright notice in the Description page of Project Settings.


#include "BreakableActor.h"

#include "GeometryCollection/GeometryCollectionComponent.h"    //需要载入GeometryCollectionEngine模块才能使用


// Sets default values
ABreakableActor::ABreakableActor()
{
	PrimaryActorTick.bCanEverTick = false;
	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	SetRootComponent(GeometryCollection);
	GeometryCollection->SetGenerateOverlapEvents(true);
}

void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABreakableActor::GetHit_Implementation(const FVector& HitPoint)
{
	IHitInterface::GetHit_Implementation(HitPoint);
}


void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

