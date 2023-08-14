// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"

void UMenuWidget::SetMenuInterface(IMenuInterface* MenuInterfaceImplementer)
{
    MenuInterface = MenuInterfaceImplementer;
}

void UMenuWidget::Setup()
{
    this->AddToViewport();

    UWorld* World = GetWorld();
    if (World == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Error: World Pointer not found for MainMenu class"));
        return;
    }

    APlayerController* PlayerController = World->GetFirstPlayerController();
    if (PlayerController == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("PlayerController pointer equals null"));
        return;
    }
    FInputModeUIOnly InputModeData;
    InputModeData.SetWidgetToFocus(this->TakeWidget());
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

    PlayerController->SetInputMode(InputModeData);
    PlayerController->bShowMouseCursor = true;

}

void UMenuWidget::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
    Super::OnLevelRemovedFromWorld(InLevel, InWorld);

    this->RemoveFromViewport();

    if (InWorld == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Error: World ptr not found for menu destruction"));
        return;
    }
    APlayerController* PlayerController = InWorld->GetFirstPlayerController();
    if (PlayerController == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Error: PlayerController ptr not found for menu destruction"));
        return;
    }

    FInputModeGameOnly InputModeData;
    PlayerController->SetInputMode(InputModeData);
    PlayerController->bShowMouseCursor = false;
}


