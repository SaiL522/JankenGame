/**
@file ServerThread.h
@brief サーバースレッド
@author Yuuho Aritomi
@date 2017/01/23
*/
#pragma once
#include <Thread.h>
#include <CriticalSection.h>
#include <list>
#include <memory>

//クライアントスレッド
class ClientThread;

/**@brief サーバースレッド*/
class ServerThread :
	public Thread
{
public:
	/**@brief コンストラクタ*/
	ServerThread(int port);
	/**@brief デストラクタ*/
	~ServerThread();
	/**@brief サーバースレッドの起動*/
	void run();
	/**@brief クライアントの追加*/
	void addClient(ClientThread* client);
	/**@brief クライアントの削除*/
	void removeClient();
	/**@brief クライアントの全て削除*/
	void clearClient();
	/**@brief クライアントにメッセージを送信*/
	void sendClient(const std::string& message);
	/**@brief サーバースレッドの終了*/
	void end();
	/**@return ポート番号*/
	int getPort() const { return mPort; }
	/**@return 終了しているか調べる*/
	bool isEnd() { return mIsEnd; }
	/**@return */
	int getConnectNum() { return mConnectNum; }
private:
	//クライアントスレッドのポインタ型
	using ClientThreadPtr = std::shared_ptr<ClientThread>;
	//クライアントスレッドのコンテナ型
	using ClientContainer = std::list<ClientThreadPtr>;
	//ポート番号
	int mPort;
	//終了フラグ
	bool mIsEnd;
	//接続人数
	ClientContainer::size_type mConnectNum;
	//クライアントスレッドコンテナ
	ClientContainer mClientContainer;
	//クライアントスレッドコンテナロック
	CriticalSection mClientContainerLock;
};

