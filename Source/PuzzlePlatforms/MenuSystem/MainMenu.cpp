// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"


bool UMainMenu::Initialize()
{
    bool Success = Super::Initialize();
    if (!Success)
    {
        return false;
    }

    if (HostButton == nullptr)
    {
        return false;
    }
    HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

    if (JoinButton == nullptr)
    {
        return false;
    }
    JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

    if (CancelJoinMenuButton == nullptr)
    {
        return false;
    }
    CancelJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

    return true;
}

void UMainMenu::SetMenuInterface(IMenuInterface* MenuInterfaceImplementer)
{
    MenuInterface = MenuInterfaceImplementer;
}

void UMainMenu::Setup()
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

void UMainMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
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

void UMainMenu::HostServer()
{
    if (MenuInterface != nullptr)
    {
        MenuInterface->Host();
    }
}

void UMainMenu::OpenJoinMenu()
{
    if (MenuSwitcher == nullptr)
    {
        return;
    }
    if (JoinMenu == nullptr)
    {
        return;
    }
    MenuSwitcher->SetActiveWidget(JoinMenu);
}



void UMainMenu::OpenMainMenu()
{
    if (MenuSwitcher == nullptr)
    {
        return;
    }
    if (MainMenu == nullptr)
    {
        return;
    }
    MenuSwitcher->SetActiveWidget(MainMenu);
}


