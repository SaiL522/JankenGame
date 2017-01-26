/**
@file ChatClientThread.h
@brief チャットクライアントスレッドクラス
@author Yuuho Aritomi
@date 2017/01/25
*/
#pragma once
#include "MessageBuffer.h"
#include <Thread.h>
#include <CriticalSection.h>
#include <Socket.h>
#include <InetSocketAddress.h>
#include <string>

/**@brief チャットクライアントスレッドクラス*/
class ChatClientThread :
	public Thread
{
public:
	/**@brief コンストラクタ*/
	ChatClientThread(unsigned int bufferSize);
	/**@brief デストラクタ*/
	~ChatClientThread();
	/**@brief ユーザー名の設定*/
	void setName(const std::string& name);
	/**@brief サーバーソケットアドレスの設定*/
	void setServerAddress(const InetSocketAddress& address);
	/**@brief スレッドの実行*/
	void run();
	/**@brief メッセージを送信*/
	void sendMessage(const std::string& message);
	/**@brief 接続中か？*/
	void printMessage(std::ostream& out);
	/**@brief 接続中か？*/
	bool isConnect();
	/**@brief 現在の接続数*/
	int connectNum();
	/**@brief 強制終了する*/
	void end();
private:
	//サーバーのソケットアドレス
	InetSocketAddress mServerAddress;
	//ユーザー名
	std::string mName;
	//接続数
	int mConnectNum;
	//メッセージバッファ
	MessageBuffer mMessageBuffer;
	//通信用ソケット
	Socket mSocket;
};

