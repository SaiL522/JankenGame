#include "MessageBuffer.h"


//コンストラクタ
MessageBuffer::MessageBuffer(unsigned int bufferSize) :
	mBufferSize(bufferSize)
{
}

//メッセージを消去
void MessageBuffer::clear()
{
	CriticalSection::Lock guard(mBufferLock);
	mBuffer.clear();
}

//メッセージを追加
void MessageBuffer::add(const std::string & message)
{
	CriticalSection::Lock guard(mBufferLock);
	//バッファにメッセージを追加
	mBuffer.push_back(message);
	//バッファサイズを超えたら１行削除
	if (mBuffer.size() > mBufferSize) {
		mBuffer.pop_front();
	}
}

//メッセージバッファの出力
void MessageBuffer::print(std::ostream & out)
{
	CriticalSection::Lock guard(mBufferLock);

	for (MessageContainer::size_type i = 0; i < mBuffer.size(); i++) {
		out << mBuffer[i];
	}
}
