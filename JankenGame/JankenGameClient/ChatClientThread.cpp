#include "ChatClientThread.h"
#include <SocketException.h>
#include <vector>
#include <string>
#include <sstream>


//コンストラクタ
ChatClientThread::ChatClientThread(unsigned int bufferSize) :
	mMessageBuffer(bufferSize),
	mConnectNum(0)
{
}

//デストラクタ
ChatClientThread::~ChatClientThread()
{
	end();
}

//ユーザー名の設定
void ChatClientThread::setName(const std::string & name)
{
	mName = name;
}

//サーバーのソケットアドレスの設定
void ChatClientThread::setServerAddress(const InetSocketAddress & address)
{
	mServerAddress = address;
}

//スレッドの実行
void ChatClientThread::run()
{
	int connectNum;
	std::string name;
	//メッセージバッファを初期化
	mMessageBuffer.clear();
	
	try {
		//サーバーに接続する
		mSocket.connect(mServerAddress);
		//ユーザー名を送信する
		sendMessage(mName);
		//接続中は受信する
		while (isConnect() == true) {
			std::string message = mSocket.receive();
			std::stringstream ssMessage(message);
			ssMessage >> connectNum >> name;

			mConnectNum = connectNum;
			if (name != mName) {
				//サーバーからのメッセージをバッファに追加
				mMessageBuffer.add(message);
			}
			//他のスレッドに制御を渡す
			Thread::sleep(0);
		}
	}
	catch (SocketException& e) {
		mMessageBuffer.add(e.what());
	}
	//ソケットを閉じる
	mSocket.close();
}

//メッセージを送信
void ChatClientThread::sendMessage(const std::string & message)
{
	//接続中でなければ送信できない
	if (isConnect() == false) return;
	//メッセージの送信を行う
	try {
		mSocket.send(message);
	}
	catch (SocketException& e) {
		mMessageBuffer.add(e.what());
	}
}

//メッセージの出力
void ChatClientThread::printMessage(std::ostream & out)
{
	mMessageBuffer.print(out);
}

bool ChatClientThread::isConnect()
{
	return mSocket.isConnected();
}

int ChatClientThread::connectNum()
{
	return mConnectNum;
}

void ChatClientThread::end()
{
	mSocket.close();
	join();
}
