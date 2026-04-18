// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueCharacter.h"

#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"


// Sets default values
ARogueCharacter::ARogueCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void ARogueCharacter::Move(const FInputActionValue& InputValue)
{
	const auto InputVector = InputValue.Get<FVector2D>();
	
	FRotator ControllerRotation = GetControlRotation();
	ControllerRotation.Pitch = 0.0f;
	
	const FVector ForwardDirection = ControllerRotation.Vector();
	const FVector RightDirection = ControllerRotation.RotateVector(FVector::RightVector);
	
	AddMovementInput(ForwardDirection, InputVector.X);
	AddMovementInput(RightDirection, InputVector.Y);
}

void ARogueCharacter::Look(const FInputActionValue& InputValue)
{
	const auto InputVector = InputValue.Get<FVector2D>();
	AddControllerPitchInput(InputVector.Y);
	AddControllerYawInput(InputVector.X);
}

// Called when the game starts or when spawned
void ARogueCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARogueCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ARogueCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent, UInputComponent>(PlayerInputComponent);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARogueCharacter::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARogueCharacter::Look);
}

