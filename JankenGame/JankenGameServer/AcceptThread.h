/**
@file AcceptThread.h
@brief アクセプトスレッドクラス
@author Yuuho Aritomi
@date 2017/01/23
*/
#pragma once
#include <ServerSocket.h>
#include <Thread.h>

//サーバースレッドクラス
class ServerThread;

/**@brief アクセプトスレッドクラス*/
class AcceptThread :
	public Thread
{
public:
	/**@brief コンストラクタ*/
	AcceptThread(ServerThread& server);
	/**@brief デストラクタ*/
	~AcceptThread();
	/**@brief スレッドの実行*/
	void run();
	/**@brief 強制終了*/
	void end();
	/**@brief 終了フラグ*/
	bool isEnd() { return mIsEnd; }
private:
	//サーバースレッド
	ServerThread& mServer;
	//サーバーソケット
	ServerSocket mServerSocket;
	//終了フラグ
	bool mIsEnd;
};

