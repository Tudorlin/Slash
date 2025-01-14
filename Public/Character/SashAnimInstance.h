
#pragma once

#include "CoreMinimal.h"
#include "CharacterTpes.h"
#include "Animation/AnimInstance.h"
#include "SashAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API USashAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly)
	class ASlashCharacter* SlashCharacter;

	UPROPERTY(BlueprintReadOnly,Category = Movement)
	class UCharacterMovementComponent* SlashCharacterMovement;

	UPROPERTY(BlueprintReadOnly,Category = Movement)
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly,Category = Movement)
	bool IsFalling;

	UPROPERTY(BlueprintReadOnly,Category="Movement | Character State")
	ECharacterState CharacterState;
	
};
