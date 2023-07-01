// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SlashCharacter.h"

#include "Camera/CameraComponent.h"
#include "GroomComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Item/Weapon/Weapon.h"

ASlashCharacter::ASlashCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	ViewCamera->SetupAttachment(CameraBoom);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName=FString("head");
	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName=FString("head");

}

void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"),this,&ASlashCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"),this,&ASlashCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"),this,&ASlashCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"),this,&ASlashCharacter::LookUp);
	
	PlayerInputComponent->BindAction(TEXT("Jump"),IE_Pressed,this,&ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Equip"),IE_Pressed,this,&ASlashCharacter::EKeyPressed);
	PlayerInputComponent->BindAction(TEXT("Attack"),IE_Pressed,this,&ASlashCharacter::Attack);
}


void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASlashCharacter::MoveForward(float Value)
{
	if(ActionState != EActionState::EAS_Unoccupied) return;
	const FRotator YawRotation(0.f,Controller->GetControlRotation().Yaw,0.f);
	const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
	AddMovementInput(Direction,Value);
}

void ASlashCharacter::MoveRight(float Value)
{
	if(ActionState != EActionState::EAS_Unoccupied) return;
	const FRotator YawRotation(0.f,Controller->GetControlRotation().Yaw,0.f);
	const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
	AddMovementInput(Direction,Value);
}

void ASlashCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ASlashCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ASlashCharacter::EKeyPressed()
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
		if( OverlappingWeapon)
		{
			OverlappingWeapon->Equip(GetMesh(),FName("RightHandSocket"));
			CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
			OverlappingItem = nullptr;
			EquippedWeapon = OverlappingWeapon;
		}
		else
		{
			if (CanDisarm())
			{
				PlayEquipMontage(FName("Unequip"));
				CharacterState = ECharacterState::ECS_Unequipped;
				ActionState = EActionState::EAS_EquippingWeapon;
			}
			else if (CanArm())
			{
				PlayEquipMontage(FName("Equip"));
				CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
				ActionState = EActionState::EAS_EquippingWeapon;
			}
		}
}

void ASlashCharacter::Attack()
{
	if(CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}

bool ASlashCharacter::CanAttack()
{
	return CharacterState != ECharacterState::ECS_Unequipped && ActionState == EActionState::EAS_Unoccupied;
}

bool ASlashCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied && 
		CharacterState != ECharacterState::ECS_Unequipped;
}

bool ASlashCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState == ECharacterState::ECS_Unequipped && 
		EquippedWeapon;
}

void ASlashCharacter::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AttackMontage&&AnimInstance)
	{
		AnimInstance->Montage_Play(AttackMontage);
		int32 Selection = FMath::RandRange(0,1);
		FName SectionName = FName();
		//随机播放攻击动画:
		switch (Selection)
		{
		case 0:
			SectionName = FName("Attack1");
			break;
		case 1:
			SectionName = FName("Attack2");
			break;
		default:
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName,AttackMontage);
	}
}

void ASlashCharacter::PlayEquipMontage(FName SectionName)
{	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(EquipMontage&&AnimInstance)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName,EquipMontage);
	}
}

void ASlashCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ASlashCharacter::DisArm()
{
	if(EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToComponent(GetMesh(),FName("SpineSpcket"));
	}
}

void ASlashCharacter::Arm()
{
	if(EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToComponent(GetMesh(),FName("RightHandSocket"));
	}
}

void ASlashCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ASlashCharacter::SetWeaponCollisionEnable(ECollisionEnabled::Type CollisionEnable)
{
	if(EquippedWeapon&&EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnable);
	}
}


void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



