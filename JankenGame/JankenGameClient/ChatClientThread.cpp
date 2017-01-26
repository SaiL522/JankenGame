#include "ChatClientThread.h"
#include <SocketException.h>
#include <vector>
#include <string>
#include <sstream>


//�R���X�g���N�^
ChatClientThread::ChatClientThread(unsigned int bufferSize) :
	mMessageBuffer(bufferSize),
	mConnectNum(0)
{
}

//�f�X�g���N�^
ChatClientThread::~ChatClientThread()
{
	end();
}

//���[�U�[���̐ݒ�
void ChatClientThread::setName(const std::string & name)
{
	mName = name;
}

//�T�[�o�[�̃\�P�b�g�A�h���X�̐ݒ�
void ChatClientThread::setServerAddress(const InetSocketAddress & address)
{
	mServerAddress = address;
}

//�X���b�h�̎��s
void ChatClientThread::run()
{
	int connectNum;
	std::string name;
	//���b�Z�[�W�o�b�t�@��������
	mMessageBuffer.clear();
	
	try {
		//�T�[�o�[�ɐڑ�����
		mSocket.connect(mServerAddress);
		//���[�U�[���𑗐M����
		sendMessage(mName);
		//�ڑ����͎�M����
		while (isConnect() == true) {
			std::string message = mSocket.receive();
			std::stringstream ssMessage(message);
			ssMessage >> connectNum >> name;

			mConnectNum = connectNum;
			if (name != mName) {
				//�T�[�o�[����̃��b�Z�[�W���o�b�t�@�ɒǉ�
				mMessageBuffer.add(message);
			}
			//���̃X���b�h�ɐ����n��
			Thread::sleep(0);
		}
	}
	catch (SocketException& e) {
		mMessageBuffer.add(e.what());
	}
	//�\�P�b�g�����
	mSocket.close();
}

//���b�Z�[�W�𑗐M
void ChatClientThread::sendMessage(const std::string & message)
{
	//�ڑ����łȂ���Α��M�ł��Ȃ�
	if (isConnect() == false) return;
	//���b�Z�[�W�̑��M���s��
	try {
		mSocket.send(message);
	}
	catch (SocketException& e) {
		mMessageBuffer.add(e.what());
	}
}

//���b�Z�[�W�̏o��
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
