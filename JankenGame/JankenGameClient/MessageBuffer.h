/**
@file MessageBuffer.h
@brief ���b�Z�[�W�𑗐M
@author Yuuho Aritomi
@date 2017/01/25
*/
#pragma once
#include <CriticalSection.h>
#include <deque>
#include <string>
#include <ostream>

/**@brief ���b�Z�[�W�o�b�t�@�N���X*/
class MessageBuffer
{
public:
	/**@brief �R���X�g���N�^*/
	MessageBuffer(unsigned int bufferSize);
	/**@brief ���b�Z�[�W������*/
	void clear();
	/**@brief ���b�Z�[�W��ǉ�*/
	void add(const std::string& message);
	/**@brief ���b�Z�[�W�o�b�t�@�̏o��*/
	void print(std::ostream& out);
private:
	//���b�Z�[�W�o�b�t�@
	using MessageContainer = std::deque<std::string>;
	//���b�Z�[�W�o�b�t�@
	MessageContainer mBuffer;
	//���b�Z�[�W�̍ő吔
	MessageContainer::size_type mBufferSize;
	CriticalSection mBufferLock;
};

