/**
@file MessageBuffer.h
@brief メッセージを送信
@author Yuuho Aritomi
@date 2017/01/25
*/
#pragma once
#include <CriticalSection.h>
#include <deque>
#include <string>
#include <ostream>

/**@brief メッセージバッファクラス*/
class MessageBuffer
{
public:
	/**@brief コンストラクタ*/
	MessageBuffer(unsigned int bufferSize);
	/**@brief メッセージを消去*/
	void clear();
	/**@brief メッセージを追加*/
	void add(const std::string& message);
	/**@brief メッセージバッファの出力*/
	void print(std::ostream& out);
private:
	//メッセージバッファ
	using MessageContainer = std::deque<std::string>;
	//メッセージバッファ
	MessageContainer mBuffer;
	//メッセージの最大数
	MessageContainer::size_type mBufferSize;
	CriticalSection mBufferLock;
};

