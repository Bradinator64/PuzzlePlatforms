// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"

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

    if (ConfirmJoinMenuButton == nullptr)
    {
        return false;
    }
    ConfirmJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

    if (QuitButton == nullptr)
    {
        return false;
    }
    QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitPressed);

    return true;
}

void UMainMenu::HostServer()
{
    if (MenuInterface != nullptr)
    {
        MenuInterface->Host();
    }
}

void UMainMenu::JoinServer()
{
    if (MenuInterface != nullptr)
    {
        if (IPAddressField == nullptr)
        {
            return;
        }
        const FString& Address = IPAddressField->GetText().ToString();
        MenuInterface->Join(Address);
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

void UMainMenu::QuitPressed()
{
    UWorld* World = GetWorld();
    if (World == nullptr)
    {
        return;
    }

    APlayerController* PlayerController = World->GetFirstPlayerController();
    if (PlayerController == nullptr)
    {
        return;
    }
    PlayerController->ConsoleCommand("quit");
}



