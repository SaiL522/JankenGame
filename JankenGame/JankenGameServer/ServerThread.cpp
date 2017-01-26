#include "ServerThread.h"
#include "ClientThread.h"
#include "AcceptThread.h"
#include <Socket.h>
#include <functional>
#include <algorithm>

//コンストラクタ
ServerThread::ServerThread(int port) :
	mPort(port),
	mIsEnd(false),
	mConnectNum(0)
{
}

//デストラクタ
ServerThread::~ServerThread()
{
	//強制終了
	end();
}

void ServerThread::run()
{
	//スレッドを開始
	mIsEnd = false;
	//アクセプトスレッドを作成する
	std::unique_ptr<AcceptThread> acceptor(new AcceptThread(*this));
	//受信スレッドを起動
	acceptor->start();
	//終了するまで繰り返し
	while (isEnd() == false)
	{
		//接続数を調べる
		mConnectNum = mClientContainer.size();
		//終了したクライアントを削除
		removeClient();
		//他のスレッドに制御を渡す
		Thread::sleep(0);
	}
	//アクセプトスレッドを全て削除
	clearClient();
	//終了フラグ
	mIsEnd = true;
}

//クライアントの追加
void ServerThread::addClient(ClientThread * client)
{
	CriticalSection::Lock guard(mClientContainerLock);
	mClientContainer.push_back(ClientThreadPtr(client));

	client->start();
}

//クライアントの削除
void ServerThread::removeClient()
{
	CriticalSection::Lock guard(mClientContainerLock);
	mClientContainer.remove_if(std::mem_fn(&ClientThread::isEnd));
	//mConnectNum--;
}

//クライアントをすべて削除
void ServerThread::clearClient()
{
	CriticalSection::Lock guard(mClientContainerLock);
	mClientContainer.clear();
	mConnectNum = 0;
}

void ServerThread::sendClient(const std::string & message)
{
	CriticalSection::Lock guard(mClientContainerLock);

	//全クライアントにメッセージを送る
	std::for_each(
		mClientContainer.begin(),
		mClientContainer.end(),
		std::bind(std::mem_fn(&ClientThread::send), std::placeholders::_1, message)
	);
}

//強制終了
void ServerThread::end()
{
	mIsEnd = true;
	join();
}
