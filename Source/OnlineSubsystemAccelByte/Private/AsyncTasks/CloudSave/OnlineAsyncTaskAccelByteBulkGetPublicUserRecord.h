// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once
#include "AsyncTasks/OnlineAsyncTaskAccelByte.h"
#include "AsyncTasks/OnlineAsyncTaskAccelByteUtils.h"
#include "OnlineSubsystemAccelByteTypes.h"
#include "OnlineCloudSaveInterfaceAccelByte.h"

/**
 * Task for getting another user record
 */
class FOnlineAsyncTaskAccelByteBulkGetPublicUserRecord : public FOnlineAsyncTaskAccelByte, public TSelfPtr<FOnlineAsyncTaskAccelByteBulkGetPublicUserRecord, ESPMode::ThreadSafe>
{
public:
	FOnlineAsyncTaskAccelByteBulkGetPublicUserRecord(FOnlineSubsystemAccelByte* const InABInterface, const FUniqueNetId& InLocalUserId, const FString& InKey, const TArray<FString>& InUserIds);

	virtual void Initialize() override;
	virtual void TriggerDelegates() override;

protected:

	virtual const FString GetTaskName() const override
	{
		return TEXT("FOnlineAsyncTaskAccelByteBulkGetPublicUserRecords");
	}

private:

	/**
	 * Delegate handler for when get another user record succeed
	 */
	void OnBulkGetPublicUserRecordSuccess(const FListAccelByteModelsUserRecord& Result);
	THandler<FListAccelByteModelsUserRecord> OnBulkGetPublicUserRecordSuccessDelegate;

	/**
	 * Delegate handler for when get another user record fails
	 */
	void OnBulkGetPublicUserRecordError(int32 Code, const FString& ErrorMessage);
	FErrorHandler OnBulkGetPublicUserRecordErrorDelegate;

	/**
	 * String representing the error code that occurred
	 */
	FText ErrorStr;

	/**
	 * String representing the record key to get
	 */
	FString Key;

	/**
	 * String representing another user id
	 */
	TArray<FString> UserIds;

	FString ErrorCode;

	FListAccelByteModelsUserRecord ListUserRecord;
};