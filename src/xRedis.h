#pragma once
#include "all.h"
#include "xEventLoop.h"
#include "xTcpconnection.h"
#include "xBuffer.h"
#include "xTcpServer.h"
#include "xSds.h"
#include "xSession.h"
#include "xObject.h"
#include "xRdb.h"
#include "xPosix.h"
#include "xLog.h"
#include "xSocket.h"
#include "xReplication.h"
#include "xSentinel.h"
#include "xCluster.h"

class xRedis : noncopyable
{
public:
	xRedis(const char * ip, int16_t port,int16_t threadCount,bool enbaledCluster = false,bool enabledSentinel = false);
	~xRedis();
	void initConifg();
	void test(void * data);
	void handleTimeOut(void * data);
	void handleSalveRepliTimeOut(void * data);
	void handleSetExpire(void * data);
	
	void run();
	void connCallBack(const xTcpconnectionPtr& conn,void *data);
	bool deCodePacket(const xTcpconnectionPtr& conn,xBuffer *recvBuf,void  *data);
	void replyCheck();
	void loadDataFromDisk();
	void flush();

	bool saveCommand(const std::deque <rObj*> & obj,xSession * session);
	bool pingCommand(const std::deque <rObj*> & obj,xSession * session);
	bool pongCommand(const std::deque <rObj*> & obj,xSession * session);
	bool ppingCommand(const std::deque <rObj*> & obj, xSession * session);
	bool ppongCommand(const std::deque <rObj*> & obj, xSession * session);
    bool debugCommand(const std::deque <rObj*> & obj, xSession * session);
	bool flushdbCommand(const std::deque <rObj*> & obj,xSession * session);
	bool dbsizeCommand(const std::deque <rObj*> & obj,xSession * session);
	bool quitCommand(const std::deque <rObj*> & obj,xSession * session);
	bool delCommand(const std::deque <rObj*> & obj,xSession * session);
	bool setCommand(const std::deque <rObj*> & obj,xSession * session);
	bool getCommand(const std::deque <rObj*> & obj,xSession * session);
	bool hsetCommand(const std::deque <rObj*> & obj,xSession * session);
	bool hgetCommand(const std::deque <rObj*> & obj,xSession * session);
	bool hgetallCommand(const std::deque <rObj*> & obj,xSession * session);
	bool hlenCommand(const std::deque <rObj*> & obj,xSession * session);
	bool slaveofCommand(const std::deque <rObj*> & obj,xSession * session);
	bool syncCommand(const std::deque <rObj*> & obj,xSession * session);
	bool psyncCommand(const std::deque <rObj*> & obj,xSession * session);
	bool commandCommand(const std::deque <rObj*> & obj,xSession * session);
	bool clusterCommand(const std::deque <rObj*> & obj,xSession * session);
	bool authCommand(const std::deque <rObj*> & obj,xSession * session);
	bool configCommand(const std::deque <rObj*> & obj,xSession * session);
	bool infoCommand(const std::deque <rObj*> & obj,xSession * session);
	bool clientCommand(const std::deque <rObj*> & obj,xSession * session);
	bool echoCommand(const std::deque <rObj*> & obj,xSession * session);
	bool subscribeCommand(const std::deque <rObj*> & obj,xSession * session);
	bool publishCommand(const std::deque <rObj*> & obj,xSession * session);
	bool selectCommand(const std::deque <rObj*> & obj,xSession * session);
	bool hkeysCommand(const std::deque <rObj*> & obj,xSession * session);
	bool scardCommand(const std::deque <rObj*> & obj,xSession * session);
	bool saddCommand(const std::deque <rObj*> & obj,xSession * session);
	bool unsubscribeCommand(const std::deque <rObj*> & obj,xSession * session);
	bool zaddCommand(const std::deque <rObj*> & obj,xSession * session);
	bool zcardCommand(const std::deque <rObj*> & obj,xSession * session);
	bool zcountCommand(const std::deque <rObj*> & obj,xSession * session);
	bool zrangeCommand(const std::deque <rObj*> & obj,xSession * session);
	bool zrankCommand(const std::deque <rObj*> & obj,xSession * session);
	bool zrevrangeCommand(const std::deque <rObj*> & obj,xSession * session);
	bool keysCommand(const std::deque <rObj*> & obj,xSession * session);
	bool bgsaveCommand(const std::deque <rObj*> & obj,xSession * session);
	bool memoryCommand(const std::deque <rObj*> & obj,xSession * session);
	bool sentinelCommand(const std::deque<rObj*> & obj, xSession * session);
	bool migrateCommand(const std::deque<rObj*> & obj, xSession * session);



	bool clearClusterMigradeCommand(void * data);
	bool save(xSession * session);
	int removeCommand(rObj * obj,int &count);
	void clearCommand();
	void clearRepliState(int32_t sockfd);
	void clearClusterState(int32_t sockfd);
	size_t getDbsize();
	void structureRedisProtocol(xBuffer &  sendBuf, std::deque<rObj*> &robjs);


public:
	
	std::unordered_set<rObj*,Hash,EEqual>  unorderedmapCommands;
	typedef std::function<bool (const std::deque<rObj*> &,xSession *)> commandFunction;
	std::unordered_map<rObj*,commandFunction,Hash,EEqual> handlerCommandMap;
	std::unordered_map<int32_t , std::shared_ptr<xSession>> sessions;

	typedef std::unordered_map<rObj*,rObj*,Hash,Equal> SetMap;
	typedef std::unordered_map<rObj*,std::unordered_map<rObj*,rObj*,Hash,Equal> ,Hash,Equal> HsetMap;
	typedef std::unordered_map<rObj*,std::unordered_set<rObj*,Hash,Equal>,Hash,Equal> Set;
	typedef std::unordered_map<rObj*,std::unordered_map<rObj *,rObj *,Hash,Equal>,Hash,Equal> SortSet;
	typedef std::unordered_map<rObj*,std::set<rSObj>,Hash,Equal> SSet;
	typedef std::unordered_map<rObj*,std::list<xTcpconnectionPtr>,Hash,Equal> PubSub;

	struct SetMapLock
	{		
		SetMap setMap;
		mutable MutexLock mutex;
	};

	struct HsetMapLock
	{
		HsetMap hsetMap;
		mutable MutexLock mutex;
	};

	struct SetLock
	{
		Set set;
		mutable MutexLock mutex;
	};

	struct SortSetLock
	{
		SortSet set;
		SSet sset;
		mutable MutexLock mutex;
	};

	struct PubSubLock
	{
		PubSub pubSub;
		mutable MutexLock mutex;
	};

	const static int kShards = 4096;
	std::array<SetMapLock, kShards> setMapShards;
	std::array<HsetMapLock, kShards> hsetMapShards;
	std::array<SetLock, kShards> setShards;
	std::array<SortSetLock,kShards> sortSetShards;
	std::array<PubSubLock, kShards> pubSubShards;

	xEventLoop loop;
	xTcpServer server;
	mutable MutexLock mutex;
	mutable MutexLock slaveMutex;
	mutable MutexLock expireMutex;
	mutable MutexLock sentinelMutex;
	mutable MutexLock clusterMutex;


	std::atomic<int>	  salveCount;
	std::atomic<bool>  clusterEnabled;
	std::atomic<bool>  slaveEnabled;
	std::atomic<bool>  authEnabled;
	std::atomic<bool>  repliEnabled;
	std::atomic<bool>  sentinelEnabled;
	std::atomic<bool>  clusterSlotEnabled;
	std::atomic<bool>  clusterRepliMigratEnabled;
	std::atomic<bool>  clusterRepliImportEnabeld;

	xBuffer	slaveCached;
	xBuffer	clusterMigratCached;
	xBuffer	clusterImportCached;
	
	xReplication  repli;
	xSentinel	   senti;
	xCluster	   clus;

	std::shared_ptr<std::thread > repliThreads;
	std::shared_ptr<std::thread > sentiThreads;
	std::shared_ptr<std::thread>  clusterThreads;

	std::unordered_map<int32_t,xTcpconnectionPtr> salvetcpconnMaps;
	std::unordered_map<int32_t, xTcpconnectionPtr> clustertcpconnMaps;
	std::unordered_map<int32_t,xTimer*> repliTimers;
	std::unordered_map<rObj*,xTimer*,Hash,Equal> expireTimers;
	
	xSocket socket;
	std::string password;
	std::atomic<int64_t >  count;
	std::atomic<bool>	pingPong;
	std::string host;
	int16_t port;
	int16_t threadCount;
	std::string masterHost;
	std::atomic<int>	masterPort ;
	
};


