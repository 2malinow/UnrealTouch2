// Fill out your copyright notice in the Description page of Project Settings.

#include "MySpectatorPawn.h"

#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Engine.h"


// Sets default values
AMySpectatorPawn::AMySpectatorPawn()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = true;
}

// Called when the game starts or when spawned
void AMySpectatorPawn::BeginPlay()
{
	Super::BeginPlay();
	playerController = UGameplayStatics::GetPlayerController(this, 0);
	SetActorRotation(FRotator(0, -90, 0));

}

void AMySpectatorPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	bool bIsFinger1CurrentlyPressed;
	playerController->GetInputTouchState(ETouchIndex::Touch1, finger1.newTouchLoc.X, finger1.newTouchLoc.Y, bIsFinger1CurrentlyPressed);
	bool bIsFinger2CurrentlyPressed;
	playerController->GetInputTouchState(ETouchIndex::Touch2, finger2.newTouchLoc.X, finger2.newTouchLoc.Y, bIsFinger2CurrentlyPressed);
	if (bIsFinger1CurrentlyPressed && !bIsFinger2CurrentlyPressed)
	{
		if (GetWorld() != nullptr)
		{
			UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
			if (ViewportClient != nullptr)
			{
				/*FVector2D deltaTouchLoc = finger1.newTouchLoc - finger1.preTouchLoc;
				FVector2D ScreenSize;
				ViewportClient->GetViewportSize(ScreenSize);

				FVector2D ScaledDelta = deltaTouchLoc / ScreenSize;;*/

				FVector2D oldpos = finger1.preTouchLoc;
				FVector oldworldpos;

				playerController->DeprojectScreenPositionToWorld(oldpos.X, oldpos.Y, oldworldpos, endLoc);
				endLoc *= 1000;

				FVector2D newpos = finger1.newTouchLoc;
				FVector newworldpos;

				playerController->DeprojectScreenPositionToWorld(newpos.X, newpos.Y, newworldpos, endLoc);

				//GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Red, FString::Printf(TEXT("startLoc: X: %f Y: %f Z: %f endLoc:  X: %f Y: %f Z: %f"), startLoc.X, startLoc.Y, startLoc.Z, endLoc.X, endLoc.Y, endLoc.Z));

				if (playerController->GetHitResultAtScreenPosition(oldpos, ECollisionChannel::ECC_Pawn, true, oldHit) && playerController->GetHitResultAtScreenPosition(newpos, ECollisionChannel::ECC_Pawn, true, newHit))
				{
					oldworldpos = oldHit.ImpactPoint;
					newworldpos = newHit.ImpactPoint;
					//DrawDebugLine(GetWorld(), startLoc, Hit.ImpactPoint, FColor(255, 0, 0), true);
					FVector offset = newworldpos-oldworldpos;
					AddActorLocalOffset(FVector(0,-offset.Y,-offset.X));
				}
				else
				{
					FVector2D deltaTouchLoc = finger1.newTouchLoc - finger1.preTouchLoc;
					FVector2D ScreenSize;
					ViewportClient->GetViewportSize(ScreenSize);

					FVector2D ScaledDelta = deltaTouchLoc / ScreenSize;
					AddActorLocalOffset(FVector(0, -deltaTouchLoc.X, deltaTouchLoc.Y));
				}



				/*if (playerController->GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Pawn, true, Hit))
				{
					DrawDebugLine(GetWorld(), startLoc, Hit.ImpactPoint, FColor(255, 0, 0), true);
					
				};*/



				//AddActorLocalOffset(FVector(0, -deltaTouchLoc.X, deltaTouchLoc.Y));
				
				
			}
		}


		finger1.preTouchLoc = finger1.newTouchLoc;
	}
	if (bIsFinger2CurrentlyPressed && !bIsFinger1CurrentlyPressed)
	{
		//nur zum testen
		/*if (GetWorld() != nullptr)
		{
			UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
			if (ViewportClient != nullptr)
			{
				FVector2D deltaTouchLoc = finger2.newTouchLoc - finger2.preTouchLoc;
				FVector2D ScreenSize;
				ViewportClient->GetViewportSize(ScreenSize);
				FVector2D ScaledDelta = deltaTouchLoc / ScreenSize;
				if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
				{
					float Value = ScaledDelta.X * BaseTurnRate;
					AddControllerYawInput(Value);
				}
				if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
				{
					float Value = ScaledDelta.Y * BaseTurnRate;
					AddControllerPitchInput(Value);
				}
			}
		}*/


		finger2.preTouchLoc = finger2.newTouchLoc;



		/*
		FVector2D deltaTouchLoc = finger2.newTouchLoc - finger2.preTouchLoc;
		GEngine->AddOnScreenDebugMessage(2, 1.0f, FColor::Green, TEXT("Touch2 wurde gedrueckt"));
		deltaTouchLoc.X *= -1.0f;

		FVector deltaOffset = FVector(deltaTouchLoc.Y, deltaTouchLoc.X, 0.0f);

		this->AddActorWorldOffset(deltaOffset);

		finger2.preTouchLoc = finger2.newTouchLoc;*/
	}
	if (bIsFinger1CurrentlyPressed&&bIsFinger2CurrentlyPressed)
	{

		if (GetWorld() != nullptr)
		{
			UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
			if (ViewportClient != nullptr)
			{
				FVector2D deltaMultiTouchLoc = finger2.newTouchLoc - finger1.newTouchLoc;
				FVector2D ScreenSize;
				ViewportClient->GetViewportSize(ScreenSize);
				FVector2D scaledDeltaMulti = deltaMultiTouchLoc / ScreenSize;
				float deltaAbsolute = sqrt(deltaMultiTouchLoc.X*deltaMultiTouchLoc.X + deltaMultiTouchLoc.Y*deltaMultiTouchLoc.Y);
				float priordeltaAbsolute = sqrt(PriorDelta.X*PriorDelta.X + PriorDelta.Y*PriorDelta.Y);
				if (deltaAbsolute > priordeltaAbsolute)
				{
					MoveForward(deltaAbsolute);
				}
				else if (deltaAbsolute < priordeltaAbsolute)
				{
					MoveForward(-deltaAbsolute);
				}
				else
				{

				}
				PriorDelta = deltaMultiTouchLoc;

			}
		}
		finger1.preTouchLoc = finger1.newTouchLoc;
		finger2.preTouchLoc = finger2.newTouchLoc;
	}
}

void AMySpectatorPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	EnableTouchscreenMovement(PlayerInputComponent);
	PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AMySpectatorPawn::OnTouchBegin);
	
}

void AMySpectatorPawn::OnTouchBegin(ETouchIndex::Type touchType, FVector newTouchLocation)
{
	if (touchType == ETouchIndex::Touch1)
	{
		bool bIsFinger1CurrentlyPressed;
		playerController->GetInputTouchState(ETouchIndex::Touch1, finger1.preTouchLoc.X, finger1.preTouchLoc.Y, bIsFinger1CurrentlyPressed);
		finger1.preTouchLoc = FVector2D(newTouchLocation.X,newTouchLocation.Y);
	}
	if (touchType == ETouchIndex::Touch2)
	{
		bool bIsFinger2CurrentlyPressed;
		playerController->GetInputTouchState(ETouchIndex::Touch2, finger2.preTouchLoc.X, finger2.preTouchLoc.Y, bIsFinger2CurrentlyPressed);
	}
}




bool AMySpectatorPawn::EnableTouchscreenMovement(UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		//		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AMySpectatorPawn::EndTouch);
		//		PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AMySpectatorPawn::TouchUpdate);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AMySpectatorPawn::OnTouchBegin);
		return true;
	}

	return false;
}
//
//void AMySpectatorPawn::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//
//	if (TouchItem.bIsPressed && TouchItem2.bIsPressed)
//	{
//		return;
//	}
//	if (TouchItem.bIsPressed && !TouchItem2.bIsPressed)
//	{
//
//		TouchItem2.bIsPressed = true;
//		TouchItem2.FingerIndex = FingerIndex;
//		TouchItem2.Location = Location;
//		TouchItem2.bMoved = false;
//		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Green, TEXT("Touch2 wurde gedrueckt"));
//		FString x = Location.ToString();
//		GEngine->AddOnScreenDebugMessage(2, 1.0f, FColor::Red, x);
//		return;
//	}
//
//	GEngine->AddOnScreenDebugMessage(3, 1.0f, FColor::Green, TEXT("Touch1 wurde gedrueckt"));
//
//	TouchItem.bIsPressed = true;
//	TouchItem.FingerIndex = FingerIndex;
//	TouchItem.Location = Location;
//	TouchItem.bMoved = false;
//	FString x = Location.ToString();
//	GEngine->AddOnScreenDebugMessage(4, 1.0f, FColor::Red, x);
//}
//
//
//void AMySpectatorPawn::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if (!TouchItem.bIsPressed && !TouchItem2.bIsPressed)
//	{
//		return;
//	}
//	if (!TouchItem.bIsPressed && TouchItem2.bIsPressed)
//	{
//		GEngine->AddOnScreenDebugMessage(5, 1.0f, FColor::Green, TEXT("Touch2 wurde losgelassen"));
//		TouchItem2.bIsPressed = false;
//		//TouchItem2.Location = FVector::ZeroVector;
//		return;
//	}
//	GEngine->AddOnScreenDebugMessage(6, 1.0f, FColor::Green, TEXT("Touch1 wurde losgelassen"));
//	TouchItem.bIsPressed = false;
//	TouchItem.bSpecial = true;
//
//}
//
//void AMySpectatorPawn::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if ((TouchItem.bIsPressed == true && TouchItem2.bIsPressed == true) &&
//		(TouchItem.FingerIndex == FingerIndex || TouchItem2.FingerIndex == FingerIndex))
//	{
//		if (GetWorld() != nullptr)
//		{
//			UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
//			if (ViewportClient != nullptr)
//			{
//				if (TouchItem.FingerIndex == FingerIndex)
//				{
//					TouchItem.Location = Location;
//				}
//				else
//				{
//					TouchItem2.Location = Location;
//				}
//				FVector MoveDelta = TouchItem2.Location - TouchItem.Location;
//
//				FVector2D ScreenSize;
//				ViewportClient->GetViewportSize(ScreenSize);
//				FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
//				FVector2D PriorScaledDelta = FVector2D(PriorDelta.X, PriorDelta.Y) / ScreenSize;
//
//				float newValue = sqrt((ScaledDelta.X*ScaledDelta.X) + (ScaledDelta.Y*ScaledDelta.Y));
//				float PriorAbsolute = sqrt((PriorScaledDelta.X*PriorScaledDelta.X) + (PriorScaledDelta.Y * PriorScaledDelta.Y));
//				GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Red, FString::Printf(TEXT("Movedelta: %f PriorDelta:  %f"), newValue, PriorAbsolute));
//
//				if (newValue > PriorAbsolute)
//				{
//					MoveForward(newValue * 100);
//
//				}
//				if (newValue < PriorAbsolute)
//				{
//					MoveForward(-newValue * 100);
//
//				}
//				else
//				{
//
//				}
//				PriorDelta = MoveDelta;
//
//			}
//
//		}
//
//	}
//	if ((TouchItem.bIsPressed) && (TouchItem.FingerIndex == FingerIndex) &&
//		(!TouchItem2.bIsPressed))
//	{
//		if (GetWorld() != nullptr)
//		{
//			UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
//			if (ViewportClient != nullptr)
//			{
//				FVector MoveDelta = Location - TouchItem.Location;
//				FVector2D ScreenSize;
//				ViewportClient->GetViewportSize(ScreenSize);
//				FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
//				if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
//				{
//					TouchItem.bMoved = true;
//					float Value = ScaledDelta.X * BaseTurnRate;
//					AddControllerYawInput(Value);
//				}
//				if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
//				{
//					TouchItem.bMoved = true;
//					float Value = ScaledDelta.Y * BaseTurnRate;
//					AddControllerPitchInput(Value);
//				}
//				TouchItem.Location = Location;
//			}
//			TouchItem.Location = Location;
//		}
//	}
//	if ((!TouchItem.bIsPressed) && ((TouchItem2.FingerIndex == FingerIndex) || (TouchItem.FingerIndex == FingerIndex)) &&
//		(TouchItem2.bIsPressed))
//	{
//		if (GetWorld() != nullptr)
//		{
//			UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
//			if (ViewportClient != nullptr)
//			{
//
//				FVector MoveDelta = Location - TouchItem2.Location;
//				FVector2D ScreenSize;
//				ViewportClient->GetViewportSize(ScreenSize);
//				FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
//				if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
//				{
//					if (TouchItem.bSpecial)
//					{
//						TouchItem2.bMoved = true;
//						float Value = ScaledDelta.X * BaseTurnRate;
//						AddControllerYawInput(Value);
//						AddControllerYawInput(-Value);
//					}
//					else
//					{
//						TouchItem2.bMoved = true;
//						float Value = ScaledDelta.X * BaseTurnRate;
//						AddControllerYawInput(Value);
//					}
//				}
//				if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
//				{
//					if (TouchItem.bSpecial)
//					{
//						TouchItem2.bMoved = true;
//						float Value = ScaledDelta.Y * BaseTurnRate;
//						AddControllerPitchInput(Value);
//						AddControllerPitchInput(-Value);
//					}
//					else
//					{
//						TouchItem2.bMoved = true;
//						float Value = ScaledDelta.Y * BaseTurnRate;
//						AddControllerPitchInput(Value);
//					}
//				}
//				TouchItem2.Location = Location;
//				TouchItem.bSpecial = false;
//			}
//			TouchItem2.Location = Location;
//		}
//	}
//
//}