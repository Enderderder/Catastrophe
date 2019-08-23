// Fill out your copyright notice in the Description page of Project Settings.

#include "CatastropheDebug.h"

#include "Engine.h"

CatastropheDebug::CatastropheDebug()
{
}

CatastropheDebug::~CatastropheDebug()
{
}

void CatastropheDebug::OnScreenDebugMsg(int32 _key, float _displayTime, FColor _color, const FString& _message, bool _bNewerOnTop, const FVector2D& _textScale)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(_key, _displayTime, _color, _message, _bNewerOnTop, _textScale);
	}
}

void CatastropheDebug::OnScreenErrorMsg(const FString& _message, float _displayTime)
{
	OnScreenDebugMsg(-1, _displayTime, FColor::Red, _message, true, FVector2D(2.0f, 2.0f));
}
