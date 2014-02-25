/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Title : MessageSystem.h
Author : Evan Wright

Description : Handles messages based on the message process function sent in.

Created :  08/12/2013
Modified : 08/12/2013

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "../Header Files/stdafx.h"
#include "../Header Files/MessageSystem.h"
#include "../Header Files/Definitions.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Gets handle to the message system
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
CMessageSystem* CMessageSystem::GetInstance(void)
{
	static CMessageSystem instance;
	return &instance;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Setup the message system processor, to the sent in function pointer.
Parameters:
[in] _msgProcessor - the function to use as the message processor
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CMessageSystem::Initialize(MESSAGEPROC _msgProcessor)
{
	// Make sure it's valid and then set the message processor!
	if(_msgProcessor)
		m_MsgProcessor = _msgProcessor;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Sends a message into the queue.
Parameters:
[in] _msg - a message to add to the message queue.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CMessageSystem::SendMessage(IMessage* _msg)
{
	// If the message exists add to message queue for processing later.
	if(_msg)
		m_MsgQueue.push(_msg);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Processes all of the messages in the queue, once per frame, using the function*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CMessageSystem::ProcessMessages(void)
{
	// If the current processor is valid
	if(m_MsgProcessor)
	{
		// While the message list isn't empty run though it!
		while(!m_MsgQueue.empty())
		{
			// Process the message
			m_MsgProcessor(m_MsgQueue.front());
			// Delete and go to the next message.
			SAFE_DELETE(m_MsgQueue.front());
			m_MsgQueue.pop();
		}
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Clears out the message queue.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CMessageSystem::ClearMessages(void)
{
	while(!m_MsgQueue.empty())
	{
		// Delete and go to the next message.
		SAFE_DELETE(m_MsgQueue.front());
		m_MsgQueue.pop();
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Clears any messages that may be left and sets the function* to nullptr.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CMessageSystem::Shutdown(void)
{
	// Clear out ALL messages.
	ClearMessages();

	// Set message processor to nullptr. *can't/don't delete a function*. o_o
	m_MsgProcessor = nullptr;
}