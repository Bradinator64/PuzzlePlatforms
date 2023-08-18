// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "EngineUtils.h"
#include "OnlineSubsystem.h"

#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuWidget.h"



UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer)
{
    ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
    if (MenuBPClass.Class == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("WBP_MainMenu class not found"));
        return;
    }

    MenuClass = MenuBPClass.Class;

    ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
    if (InGameMenuBPClass.Class == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("WBP_InGameMenu class not found"));
        return;
    }

    InGameMenuClass = InGameMenuBPClass.Class;
}

void UPuzzlePlatformsGameInstance::Init()
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Found Online Sub-System: %s"), *Subsystem->GetSubsystemName().ToString());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Online Sub-System not found"));
    }
}

void UPuzzlePlatformsGameInstance::LoadMenuWidget()
{
    if (MenuClass == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("MenuClass not set"));
        return;
    }
    UMainMenu* Menu = CreateWidget<UMainMenu>(this, MenuClass);
    if (Menu == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Menu ptr not set"));
        return;
    }

    Menu->Setup();


    Menu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::InGameLoadMenu()
{
    if (InGameMenuClass == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("InGameMenuClass not set"));
        return;
    }
    UMenuWidget* InGameMenu = CreateWidget<UMenuWidget>(this, InGameMenuClass);
    if (InGameMenu == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Menu ptr not set"));
        return;
    }

    InGameMenu->Setup();

    InGameMenu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::Host()
{
    UEngine* Engine = GetEngine();
    if (Engine == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Engine Pointer Null"));
        return;
    }

    Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

    UWorld* World = GetWorld();
    if (World == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("World Pointer: null"));
        Engine->AddOnScreenDebugMessage(0, 3, FColor::Red, TEXT("Error: World Pointer equals null"));
        return;
    }

    World->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap?listen");

}

void UPuzzlePlatformsGameInstance::Join(const FString& Address)
{
    UEngine* Engine = GetEngine();
    if (Engine == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Engine Pointer Null"));
        return;
    }

    Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

    APlayerController* PlayerController = GetFirstLocalPlayerController();
    if (PlayerController == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("PlayerController pointer equals null"));
        Engine->AddOnScreenDebugMessage(0, 3, FColor::Red, TEXT("Error: PlayerController pointer equals null"));
        return;
    }

    PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformsGameInstance::LoadMainMenu()
{
    APlayerController* PlayerController = GetFirstLocalPlayerController();
    if (PlayerController == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("PlayerController pointer equals null"));
        return;
    }
    //if the player controller is the host
    if (PlayerController->HasAuthority())
    {
        for (APlayerController* Controller : TActorRange<APlayerController>(GetWorld()))
        {
            if (Controller != PlayerController)
            {
                //call ClientTravel() on any controllers that are not the host's controller
                Controller->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
            }
        }
    }
    PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}


