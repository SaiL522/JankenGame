/**
@file ClientThread.h
@brief クライアントスレッドクラス
@author Yuuho Aritomi
@date 2017/01/23
*/
#pragma once
#include <Thread.h>
#include <string>
#include <memory>

//サーバースレッドクラス
class ServerThread;

//ソケットクラス
class Socket;

/**@brief クライアントスレッドクラス*/
class ClientThread :
	public Thread
{
public:
	/**@brief コンストラクタ*/
	ClientThread(ServerThread& server, Socket* socket, const std::string& name);
	/**@brief デストラクタ*/
	~ClientThread();
	/**@brief スレッドの起動*/
	void run();
	/**@brief メッセージを送信*/
	void send(const std::string& message);
	/**@brief 強制終了*/
	void end();
	/**@brief スレッドが終了しているか？*/
	bool isEnd() { return mIsEnd; }
private:
	//サーバースレッド
	ServerThread& mServer;
	//クライアント用の送受信ソケット
	std::unique_ptr<Socket> mSocket;
	//ユーザー名
	std::string mName;
	//終了フラグ
	bool mIsEnd;
};

