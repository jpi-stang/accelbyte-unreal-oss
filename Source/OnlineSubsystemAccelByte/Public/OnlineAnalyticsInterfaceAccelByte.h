﻿// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "OnlineUserCacheAccelByte.h"
#include "Api/AccelByteGameTelemetryApi.h"
#include "GameServerApi/AccelByteServerGameTelemetryApi.h"

/**
 * Implementation of Analytics service from AccelByte services
 */
class ONLINESUBSYSTEMACCELBYTE_API FOnlineAnalyticsAccelByte : public TSharedFromThis<FOnlineAnalyticsAccelByte, ESPMode::ThreadSafe>
{
PACKAGE_SCOPE:
	FOnlineAnalyticsAccelByte(FOnlineSubsystemAccelByte* InSubsystem)
		: AccelByteSubsystem(InSubsystem)
	{}

public:
	virtual ~FOnlineAnalyticsAccelByte() {};

	/**
	 * Convenience method to get an instance of this interface from the subsystem associated with the world passed in.
	 *
	 * @param World World instance that we wish to get the interface from
	 * @param OutInterfaceInstance Instance of the interface that we got from the subsystem, or nullptr if not found
	 * @returns boolean that is true if we could get an instance of the interface, false otherwise
	 */
	static bool GetFromWorld(const UWorld* World, TSharedPtr<FOnlineAnalyticsAccelByte, ESPMode::ThreadSafe>& OutInterfaceInstance);

	/**
	 * Convenience method to get an instance of this interface from the subsystem passed in.
	 *
	 * @param Subsystem Subsystem instance that we wish to get this interface from
	 * @param OutInterfaceInstance Instance of the interface that we got from the subsystem, or nullptr if not found
	 * @returns boolean that is true if we could get an instance of the interface, false otherwise
	 */
	static bool GetFromSubsystem(const IOnlineSubsystem* Subsystem, TSharedPtr<FOnlineAnalyticsAccelByte, ESPMode::ThreadSafe>& OutInterfaceInstance);

	/**
	 * Set GameTelemetry or ServerGameTelemetry send event interval from DefaultEngine.ini
	 *
	 * @param InLocalUserNum user identifier
	 * @returns boolean that is true if task for setting send interval successfully dispatched
	 */
	bool SetTelemetrySendInterval(int32 InLocalUserNum);

	/**
	 * Set GameTelemetry or ServerGameTelemetry immediate event list
	 *
	 * @param InLocalUserNum user identifier
	 * @param EventNames list of immediate event name 
	 * @returns boolean that is true if task for setting immediate event lists successfully dispatched
	 */
	bool SetTelemetryImmediateEventList(int32 InLocalUserNum, TArray<FString> const& EventNames);

	/**
	 * Send GameTelemetry or ServerGameTelemetry event with delegates OnSuccess and OnError
	 *
	 * @param InLocalUserNum user identifier
	 * @param TelemetryBody content of telemetry event
	 * @param OnSuccess delegate will be called after event send succeed
	 * @param OnError delegate will be called after event send error
	 * @returns boolean that is true if task for sending telemetry event successfully dispatched
	 */
	bool SendTelemetryEvent(
		int32 InLocalUserNum, FAccelByteModelsTelemetryBody const& TelemetryBody,
		AccelByte::FVoidHandler const& OnSuccess, AccelByte::FErrorHandler const& OnError);

protected:
	/** Hidden default constructor, the constructor that takes in a subsystem instance should be used instead. */
	FOnlineAnalyticsAccelByte()
		: AccelByteSubsystem(nullptr)
	{}
	
	/** Instance of the subsystem that created this interface */
	FOnlineSubsystemAccelByte* AccelByteSubsystem = nullptr;

private:
	/**
	 * Helper function to check if user logged in
	 *
	 * @param InLocalUserNum user identifier
	 * @return boolean that is true if the user logged in
	 */
	bool IsUserLoggedIn(const int32 InLocalUserNum) const;

	/**
	 * Helper function to check if Telemetry body is valid
	 *
	 * @param TelemetryBody telemetry object
	 * @return boolean that is true if the telemetry object is valid
	 */
	static bool IsValidTelemetry(FAccelByteModelsTelemetryBody const& TelemetryBody);
};
