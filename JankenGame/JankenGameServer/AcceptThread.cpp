#include "AcceptThread.h"
#include "ServerThread.h"
#include "ClientThread.h"
#include <Socket.h>
#include <SocketException.h>
#include <InetAddress.h>
#include <iostream>
#include <memory>


//コンストラクタ
AcceptThread::AcceptThread(ServerThread & server) :
	mServer(server),
	mIsEnd(false)
{
}

//デストラクタ
AcceptThread::~AcceptThread()
{
	end();
}

//スレッドの起動
void AcceptThread::run()
{
	mIsEnd = false;

	try {
		mServerSocket.bind(mServer.getPort());

		while (isEnd() == false) {
			std::cout << "クライアント接続待ち" << std::endl;
			//クライアントを受け付ける
			std::unique_ptr<Socket> client(mServerSocket.accept());
			std::cout << client->getAddress().getHostAddress() << "接続しました" << std::endl;
			std::string name = client->receive();
			//サーバーにクライアントを追加する
			mServer.addClient(new ClientThread(mServer, client.release(), name));
			//他のスレッドに制御を渡す
			Thread::sleep(0);
		}
	}
	catch (SocketException&) {

	}
	mServerSocket.close();
	mIsEnd = true;
}

//強制終了
void AcceptThread::end()
{
	mIsEnd = true;
	mServerSocket.close();
	join();
}
