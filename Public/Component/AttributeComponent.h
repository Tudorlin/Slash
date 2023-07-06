// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLASH_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ReceiveDamage(float Damage);
	float GetHealthPercet();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere,Category="Actor Attritubes")
	float Health;

	UPROPERTY(EditAnywhere,Category="Actor Attritubes")
	float MaxHealth;

public:	

		
};
