

#include "Component/AttributeComponent.h"

UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}


void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	
	
}


void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

void UAttributeComponent::ReceiveDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage,0.f,MaxHealth);
}

float UAttributeComponent::GetHealthPercet()
{
	return Health/MaxHealth;
}

bool UAttributeComponent::IsAlive()
{
	return Health > 0;
}

