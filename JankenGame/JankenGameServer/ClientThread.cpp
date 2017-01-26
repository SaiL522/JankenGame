#include "ClientThread.h"
#include "ServerThread.h"
#include <Socket.h>

//コンストラクタ
ClientThread::ClientThread(ServerThread & server, Socket * socket, const std::string & name) :
	mServer(server),
	mSocket(socket),
	mName(name),
	mIsEnd(false)
{
}

//デストラクタ
ClientThread::~ClientThread()
{
	end();
}

//スレッドの起動
void ClientThread::run()
{
	int connectNum;
	mIsEnd = false;
	try {
		//クライアントからのメッセージを処理する
		while (mIsEnd == false) {
			//クライアントからのデータを受信する
			std::string message = mSocket->receive();
			//クライアントの接続要求をチェック
			if (message == "-2") {
				break;
			}
			//ユーザー名を付加して、クライアントにメッセージを送信
			mServer.sendClient(
				std::to_string(mServer.getConnectNum()) + " " + mName  + " " + message);
			//他のスレッドに制御を渡す
			Thread::sleep(0);
		}
	} 
	catch (SocketException&) {

	}
	mIsEnd = true;
}

void ClientThread::send(const std::string & message)
{
	mSocket->send(message);
}

//強制終了
void ClientThread::end()
{
	mIsEnd = true;
	mSocket->close();
	join();
}