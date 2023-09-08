// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/C_BossUI.h"
#include "Components/ProgressBar.h"

#include "Characters/C_Unit.h"

UC_BossUI::UC_BossUI(const FObjectInitializer& ObjectInitializer)
	:UUserWidget(ObjectInitializer)
{
	
}

void UC_BossUI::NativeConstruct()
{
	Super::NativeConstruct();

	TimerDelegate.BindUFunction(this, "Shaking");
}

void UC_BossUI::NativeDestruct()
{
	Super::NativeDestruct();
}

void UC_BossUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	HP->SetPercent(UnitStatus->GetHPRate());
}

void UC_BossUI::Update()
{
	ShakeUI();
}

void UC_BossUI::Show(FUnitStatus* Status)
{
	SetVisibility(ESlateVisibility::Visible);
	
	UnitStatus = Status;
}

void UC_BossUI::Hide()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UC_BossUI::ShakeUI()
{
	ClearTimer();
	GetWorld()->GetTimerManager().SetTimer(ShakingHandle, TimerDelegate, TimerInterval, true);
}

void UC_BossUI::Shaking()
{
	if (ShakingTime < 1.0f)
	{
		ShakingTime += TimerInterval;

		if (ShakingSwitch)
			ShakingAxis += ShakingPower;
		else
			ShakingAxis -= ShakingPower;

		if (fabs(ShakingAxis) > ShakingRange)
			ShakingSwitch = !ShakingSwitch;

		SetHPWidgetTransform(ShakingAxis);
	}
	else
	{
		ClearTimer();
	}
}

void UC_BossUI::ClearTimer()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(ShakingHandle))
		GetWorld()->GetTimerManager().ClearTimer(ShakingHandle);

	ShakingAxis = 0.0f;
	ShakingTime = 0.0f;

	SetHPWidgetTransform(ShakingAxis);
}

void UC_BossUI::SetHPWidgetTransform(float TranslationAxisY)
{
	FWidgetTransform transform;
	transform.Translation.Y = TranslationAxisY;

	HP->SetRenderTransform(transform);
}