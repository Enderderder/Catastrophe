// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * This is a helper function collection of the engine debug methods
 */
class CATASTROPHE_API CatastropheDebug
{
public:
	CatastropheDebug();
	~CatastropheDebug();

	/**
	 * This function calls the engine AddOnScreenDebugMessage
	 * The purpose is to reduce the compile time as to use the function it needs to
	 * include the Engine.h file which is taking a long time as it takes long time to compile
	 * @author Richard Wulansari
	 * @param _key The key of which position of the message will be
	 * @param _displayTime The time of displaying the message
	 * @param _message The message to display
	 * @param _bNewerOnTop If the message will be displayed on top of other messages, ONLY works when the _key == INDEX_NONE
	 * @param _textScale The scale of the message to be display
	 */
	static void OnScreenDebugMsg(int32 _key, float _displayTime, FColor _color, const FString& _message, bool _bNewerOnTop = true, const FVector2D& _textScale = FVector2D::UnitVector);


	/**
	 * Calls the engine AddOnScreenDebugMessage wuth Red color
	 * @author Richard Wulansari
	 * @param _message The message to display
	 * @param _displayTime The time of displaying the message
	 */
	static void OnScreenErrorMsg(const FString& _message, float _displayTime = 10.0f);


};
