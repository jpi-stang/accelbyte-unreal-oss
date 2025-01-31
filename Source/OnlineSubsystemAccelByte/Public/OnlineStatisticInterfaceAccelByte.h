// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.
#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemTypes.h"
#include "OnlineDelegateMacros.h"
#include "OnlineSubsystemAccelByte.h"
#include "OnlineUserCacheAccelByte.h"
#include "Models/AccelByteStatisticModels.h"
#include "Interfaces/OnlineStatsInterface.h"

DECLARE_MULTICAST_DELEGATE_FourParams(FOnListUserStatItemsCompleted, int32 /*LocalUserNum*/, bool /*bWasSuccessful*/, const TArray<FAccelByteModelsFetchUser>&, const FString& /*Error*/);
typedef FOnListUserStatItemsCompleted::FDelegate FOnListUserStatItemsCompletedDelegate;

DECLARE_MULTICAST_DELEGATE_FourParams(FOnUserStatItemsResetCompleted, int32 /*LocalUserNum*/, bool /*bWasSuccessful*/, const TArray<FAccelByteModelsUpdateUserStatItemsResponse>&, const FString& /*Error*/);
typedef FOnUserStatItemsResetCompleted::FDelegate FOnUserStatItemsResetCompletedDelegate;

DECLARE_DELEGATE_TwoParams(FOnlineStatsCreateStatsComplete, const FOnlineError& /*ResultState*/, const TArray<FAccelByteModelsBulkStatItemOperationResult>& /*Result*/);

/**
 * Implementation of Statistic service from AccelByte services
 */
class ONLINESUBSYSTEMACCELBYTE_API FOnlineStatisticAccelByte : public IOnlineStats, public TSharedFromThis<FOnlineStatisticAccelByte, ESPMode::ThreadSafe>
{ 
PACKAGE_SCOPE:
	/* Map of Users */
	TUniqueNetIdMap<TArray<TSharedRef<FAccelByteModelsFetchUser>>> UsersMap;
	
	/** Constructor that is invoked by the Subsystem instance to create a user cloud instance */
	FOnlineStatisticAccelByte(FOnlineSubsystemAccelByte* InSubsystem)
		: AccelByteSubsystem(InSubsystem)
	{}

public:
	virtual ~FOnlineStatisticAccelByte() override = default;

	bool ListUserStatItems(int32 LocalUserNum, const TArray<FString>& StatCodes, const TArray<FString>& Tags, const FString& AdditionalKey, bool bAlwaysRequestToService);

	DEFINE_ONLINE_PLAYER_DELEGATE_THREE_PARAM(MAX_LOCAL_PLAYERS, OnListUserStatItemsCompleted, bool, const TArray<FAccelByteModelsFetchUser>&, const FString&);

	DEFINE_ONLINE_PLAYER_DELEGATE_THREE_PARAM(MAX_LOCAL_PLAYERS, OnUserStatItemsResetCompleted, bool, const TArray<FAccelByteModelsUpdateUserStatItemsResponse>&, const FString&);

	/* Get the list users stat items */
	// TODO : make this method as implementation from GetStats
	bool GetListUserStatItems(int32 LocalUserNum, TArray<TSharedRef<FAccelByteModelsFetchUser>>& OutUsers);
	TSharedPtr<const FOnlineStatsUserStats>  GetAllListUserStatItemFromCache(const FUniqueNetIdRef StatsUserId) const;

	/**
	 * Query a specific user's stats
	 *
	 * @param LocalUserId User to query as (if applicable)
	 * @param StatsUser User to get stats for
	 * @param StatNames Stats to get stats for all specified users
	 * @param Delegate Called when the user's stats have finished being requested and are now available, or when we fail to retrieve the user's stats
	 */
	void QueryStats(const FUniqueNetIdRef LocalUserId, const FUniqueNetIdRef StatsUser, const TArray<FString>& StatNames, const FOnlineStatsQueryUserStatsComplete& Delegate);

	/**
	 * Query a specific user's stats
	 *
	 * @param LocalUserNum Index of user(or server) to query as
	 * @param StatsUser User to get stats for
	 * @param StatNames Stats to get stats for all specified users
	 * @param Delegate Called when the user's stats have finished being requested and are now available, or when we fail to retrieve the user's stats
	 */
	virtual void QueryStats(const int32 LocalUserNum, const FUniqueNetIdRef StatsUser, const TArray<FString>& StatNames, const FOnlineStatsQueryUserStatsComplete& Delegate);

	/**
	 * Query a specific user's stats
	 *
	 * @param LocalUserNum Index of user(or server) to query as
	 * @param StatsUser User to get stats for
	 * @param Delegate Called when the user's stats have finished being requested and are now available, or when we fail to retrieve the user's stats
	 */
	virtual void QueryStats(const int32 LocalUserNum, const FUniqueNetIdRef StatsUser, const FOnlineStatsQueryUserStatsComplete& Delegate);

	/**
	 * Query a one or more user's stats
	 *
	 * @param LocalUserNum Index of user(or server) to query as
	 * @param StatsUser Users to get stats for
	 * @param StatNames Stats to get stats for all specified users
	 * @param Delegate Called when the user's stats have finished being requested and are now available, or when we fail to retrieve the user's stats
	 */
	virtual void QueryStats(const int32 LocalUserNum, const TArray<FUniqueNetIdRef>& StatUsers, const TArray<FString>& StatNames, const FOnlineStatsQueryUsersStatsComplete& Delegate);

	/**
	 * Create a one or more user's stats. Only for request by Game Server
	 * 
	 * @param LocalUserNum Index of user(server) that is attempting to create the stats
	 * @param StatsUser User to create stats for
	 * @param StatNames Stats to create stats for all specified users
	 * @param Delegate Called when the user's stats have finished being created, or when we fail to create the user's stats
	 */
	virtual void CreateStats(const int32 LocalUserNum, const FUniqueNetIdRef StatsUser, const TArray<FString>& StatNames, const FOnlineStatsCreateStatsComplete& Delegate);
	
	/**
	 * Emplace a user's cached stats object
	 *
	 * @param UserStats The stats to emplace
	 */
	virtual void EmplaceStats(const TSharedPtr<const FOnlineStatsUserStats>& InUserStats);

	/**
	 * Emplace user's cached stats object
	 *
	 * @param UsersStats The stats to emplace
	 */
	virtual void EmplaceStats(const TArray<TSharedPtr<const FOnlineStatsUserStats>>& InUsersStats);

	/**
	 * Reset all user statistics.
	 *
	 * @param LocalUserNum Index of user(server) that is attempting to create the stats
	 * @param StatsUserId User to create stats for
	 */
	virtual void ResetStats(const int32 LocalUserNum, const FUniqueNetIdRef StatsUserId);

	//~ Begin IOnlineStats Interface
	virtual void QueryStats(const FUniqueNetIdRef LocalUserId, const FUniqueNetIdRef StatsUser, const FOnlineStatsQueryUserStatsComplete& Delegate) override;
	virtual void QueryStats(const FUniqueNetIdRef LocalUserId, const TArray<FUniqueNetIdRef>& StatUsers, const TArray<FString>& StatNames, const FOnlineStatsQueryUsersStatsComplete& Delegate) override;
	virtual TSharedPtr<const FOnlineStatsUserStats> GetStats(const FUniqueNetIdRef StatsUserId) const override;
	virtual void UpdateStats(const FUniqueNetIdRef LocalUserId, const TArray<FOnlineStatsUserUpdatedStats>& UpdatedUserStats, const FOnlineStatsUpdateStatsComplete& Delegate) override;
#if !UE_BUILD_SHIPPING
	virtual void ResetStats(const FUniqueNetIdRef StatsUserId) override;
#endif // !UE_BUILD_SHIPPING
	//~ End  IOnlineStats Interface

protected:
	/** Hidden default constructor, the constructor that takes in a subsystem instance should be used instead. */
	FOnlineStatisticAccelByte()
		: AccelByteSubsystem(nullptr)
	{}

	/** Instance of the subsystem that created this interface */
	FOnlineSubsystemAccelByte* AccelByteSubsystem = nullptr;

private :
	/**
	 * Query a specific user's stats
	 *
	 * @param LocalUserNum Index of user(or server) to query as
	 * @param LocalUserId User to query as (if applicable)
	 * @param StatsUser User to get stats for
	 * @param StatNames Stats to create stats for all specified users
	 * @param Delegate Called when the user's stats have finished being requested and are now available, or when we fail to retrieve the user's stats
	 */
	virtual void QueryStats(const int32 LocalUserNum, const FUniqueNetId& LocalUserId, const FUniqueNetIdRef StatsUser, const TArray<FString>& StatNames, const FOnlineStatsQueryUserStatsComplete& Delegate);
	
	TSharedPtr<const FOnlineStatsUserStats> UserStats;
	/** Critical sections for thread safe operation of UsersStats */
	mutable FCriticalSection StatsLock;
	TArray<TSharedRef<const FOnlineStatsUserStats>> UsersStats;
};