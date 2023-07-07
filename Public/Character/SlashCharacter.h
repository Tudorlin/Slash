// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterTpes.h"
#include "GameFramework/Character.h"
#include "SlashCharacter.generated.h"

UCLASS()
class SLASH_API ASlashCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASlashCharacter();
	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	

protected:
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void EKeyPressed();
	void Attack();

	bool CanAttack();
	bool CanDisarm();
	bool CanArm();

//蒙太奇动画播放
	void PlayAttackMontage();
	void PlayEquipMontage(const FName SectionName);
	

	UFUNCTION(BlueprintCallable)
	void AttackmontageEnd();      //可在蓝图中调用的函数来重置角色的攻击状态

	UFUNCTION(BlueprintCallable)
	void DisArm();

	UFUNCTION(BlueprintCallable)
	void Arm();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnable(ECollisionEnabled::Type CollisionEnable);

	UPROPERTY(VisibleInstanceOnly)
	class AItem* OverlappingItem;  //与Character类重叠的Item类，用于检测是否是能被装备的武器

private:
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* ViewCamera;
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleAnywhere,Category=Hair)
	class UGroomComponent* Hair;
	UPROPERTY(VisibleAnywhere,Category=Hair)
	UGroomComponent* Eyebrows;

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite,meta=(AllowPrivateAccess = "true"))           //由于要在动画蓝图中使用通知重置攻击状态,将此变量公开至动画蓝图
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY(EditDefaultsOnly,Category=Montage)
	UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly,Category=Montage)
	UAnimMontage* EquipMontage;

	UPROPERTY(VisibleAnywhere,Category=Weapon)
	class AWeapon* EquippedWeapon;

	

public:
	FORCEINLINE void SetOverlappingWeapon(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }  //返回角色状态，在动画实例中调用以判断角色状态从而决定动画的播放

	
	
};
