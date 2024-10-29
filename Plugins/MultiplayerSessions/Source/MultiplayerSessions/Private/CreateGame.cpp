// Fill out your copyright notice in the Description page of Project Settings.


#include "CreateGame.h"
#include "Components/CheckBox.h"
#include "Components/EditableTextBox.h"

void UCreateGame::NativeConstruct()
{
    if (FreeForAllCheckbox)
    {
        FreeForAllCheckbox->OnCheckStateChanged.AddDynamic(this, &ThisClass::FreeForAllCheckboxCheckStateChanged);
    }
    if (TeamsCheckbox)
    {
        TeamsCheckbox->OnCheckStateChanged.AddDynamic(this, &ThisClass::TeamsCheckboxCheckboxCheckStateChanged);
    }
    if (CaptureTheFlagCheckbox)
    {
        if (CaptureTheFlagCheckbox)
            CaptureTheFlagCheckbox->OnCheckStateChanged.AddDynamic(this, &ThisClass::CaptureTheFlagCheckboxCheckboxCheckStateChanged);
    }
    if (GhostCheckbox)
    {
        GhostCheckbox->OnCheckStateChanged.AddDynamic(this, &ThisClass::GhostCheckboxCheckboxCheckStateChanged);
    }
    if (NOFTextBox)
    {
        // Vincula la función al evento de cambio de texto
        NOFTextBox->OnTextChanged.AddDynamic(this, &ThisClass::OnTextChanged);
        NOFTextBox->OnTextCommitted.AddDynamic(this, &ThisClass::OnTextCommitted);
    }
}

void UCreateGame::FreeForAllCheckboxCheckStateChanged(bool bIsChecked)
{
    if (!bIsChecked)
    {
     FreeForAllCheckbox->SetCheckedState(ECheckBoxState::Checked);
    }
    TeamsCheckbox->SetCheckedState(ECheckBoxState::Unchecked);
    CaptureTheFlagCheckbox->SetCheckedState(ECheckBoxState::Unchecked);
    GhostCheckbox->SetCheckedState(ECheckBoxState::Unchecked);
    MatchType = TEXT("FreeForAll");
}

void UCreateGame::TeamsCheckboxCheckboxCheckStateChanged(bool bIsChecked)
{
    if (!bIsChecked)
    {
        TeamsCheckbox->SetCheckedState(ECheckBoxState::Checked);
    }
    FreeForAllCheckbox->SetCheckedState(ECheckBoxState::Unchecked);
    CaptureTheFlagCheckbox->SetCheckedState(ECheckBoxState::Unchecked);
    GhostCheckbox->SetCheckedState(ECheckBoxState::Unchecked);
    MatchType = TEXT("Teams");
}

void UCreateGame::CaptureTheFlagCheckboxCheckboxCheckStateChanged(bool bIsChecked)
{
    if (!bIsChecked)
    {
        CaptureTheFlagCheckbox->SetCheckedState(ECheckBoxState::Checked);
    }
    FreeForAllCheckbox->SetCheckedState(ECheckBoxState::Unchecked);
    TeamsCheckbox->SetCheckedState(ECheckBoxState::Unchecked);
    GhostCheckbox->SetCheckedState(ECheckBoxState::Unchecked);
    MatchType = TEXT("CaptureTheFlag");
}

void UCreateGame::GhostCheckboxCheckboxCheckStateChanged(bool bIsChecked)
{
    if (!bIsChecked)
    {
        GhostCheckbox->SetCheckedState(ECheckBoxState::Checked);
    }
    FreeForAllCheckbox->SetCheckedState(ECheckBoxState::Unchecked);
    CaptureTheFlagCheckbox->SetCheckedState(ECheckBoxState::Unchecked);
    TeamsCheckbox->SetCheckedState(ECheckBoxState::Unchecked);
    MatchType = TEXT("Ghost");
}

void UCreateGame::OnTextChanged(const FText& Text)
{
    FString InputString = Text.ToString();

    // Valida que solo se ingresen números
    for (int32 i = 0; i < InputString.Len(); ++i)
    {
        if (!FChar::IsDigit(InputString[i]))
        {
            // Remueve el carácter no numérico
            InputString.RemoveAt(i);
            --i; // Ajusta el índice después de eliminar un carácter
        }
    }

    // Actualiza el texto solo con números
    NOFTextBox->SetText(FText::FromString(InputString));
    NumPublicConnections = FCString::Atoi(*InputString);
}

void UCreateGame::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
    FString InputString = Text.ToString();

    // Si está vacío, reescribe con "1"
    if (InputString.IsEmpty())
    {
        NOFTextBox->SetText(FText::FromString(TEXT("1")));
        NumPublicConnections = FCString::Atoi(*InputString);
    }
}

