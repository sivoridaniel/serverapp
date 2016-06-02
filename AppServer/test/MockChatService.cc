#include "gmock/gmock.h"
#include "../src/service/impl/ChatService.h"
#include "../src/dao/ChatDao.h"
#include <string>
#include <ctime>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>

using namespace std;
using ::testing::AtLeast;
using ::testing::Return;
using ::testing::_;



class MockChatDao : public ChatDao{
public:

	MOCK_METHOD1(get, Entity*(string id) );

	MOCK_METHOD2(put, void(string id, Entity* e) );

};

ACTION(ThrowEntityNotFoundException){
	throw EntityNotFoundException();
}

ACTION(ThrowEntityExistsException){
	throw EntityExistsException();
}

/**
 * Test para probar el camino feliz cuando se obtienen todos los mensajes de un chat
 */
TEST(ChatServiceGetAllMessagesTest,getAllMessages){
	MockChatDao* mockChatDao = new MockChatDao();

	Chat* chat = new Chat();
	chat->addNewMessage("psivori", "hola");
	chat->addNewMessage("jferrio", "chau");
	EXPECT_CALL(*mockChatDao, get("jferrio-psivori")).Times(AtLeast(1)).WillOnce(Return(chat));
	ChatService* chatService = new ChatService(mockChatDao);
	vector<Message*> messages;
	EXPECT_NO_THROW({messages = chatService->getAllMessages("jferrio", "psivori");});
	ASSERT_TRUE(messages.size()==2);
	for (vector<Message*>::iterator it=messages.begin(); it!=messages.end(); ++it){
		Message* message = *it;
		delete message;
	}
	delete chatService;
}

/**
 * Test para probar el caso de que no exista el chat buscado
 */
TEST(ChatServiceGetAllMessagesNonexistentChatTest,getAllMessagesNonexistentChat){
	MockChatDao* mockChatDao = new MockChatDao();

	ChatService* chatService = new ChatService(mockChatDao);
	EXPECT_CALL(*mockChatDao, get("alinari-psivori")).Times(1).WillRepeatedly(ThrowEntityNotFoundException());
	try{
		chatService->getAllMessages("alinari", "psivori");
		FAIL();
	}
	catch(EntityNotFoundException& e){
		//OK;
	}
	catch(exception& e){
		FAIL();
	}
	delete chatService;
}


/**
 * Test para probar el camino feliz cuando se obtienen los mensajes nuevos para el usuario1 de un chat
 */
TEST(ChatServiceGetNewMessagesTest,getNewMessages){
	MockChatDao* mockChatDao = new MockChatDao();

	Chat* chat = new Chat();
	chat->addNewMessage("psivori", "hola");
	chat->addNewMessage("jferrio", "chau");
	chat->addNewMessage("jferrio", "chauuu");
	chat->setIdUser1("jferrio");
	chat->setIdUser2("psivori");
	chat->setLastSeenByUser1(1);
	chat->setLastSeenByUser2(0);
	EXPECT_CALL(*mockChatDao, get("jferrio-psivori")).Times(AtLeast(1)).WillOnce(Return(chat));
	ChatService* chatService = new ChatService(mockChatDao);
	vector<Message*> messages;

	EXPECT_NO_THROW({messages = chatService->getNewMessages("jferrio","psivori");});
	ASSERT_TRUE(messages.size()==2);
	for (vector<Message*>::iterator it=messages.begin(); it!=messages.end(); ++it){
		Message* message = *it;
		delete message;
	}
	delete chatService;
}

/**
 * Test para probar el caso de que no exista el chat buscado
 */
TEST(ChatServiceGetNewMessagesNonexistentChatTest,getNewMessagesNonexistentChat){
	MockChatDao* mockChatDao = new MockChatDao();
	EXPECT_CALL(*mockChatDao, get("alinari-psivori")).Times(1).WillRepeatedly(ThrowEntityNotFoundException());

	ChatService* chatService = new ChatService(mockChatDao);
	try{
		chatService->getNewMessages("alinari", "psivori");
		FAIL();
	}
	catch(EntityNotFoundException& e){
		//OK;
	}
	catch(exception& e){
		FAIL();
	}
	delete chatService;
}


/**
 * Test para probar el camino feliz cuando se postea un nuevo mensaje
 */
TEST(ChatServiceAddNewMessageTest,addNewMessage){
	MockChatDao* mockChatDao = new MockChatDao();

	Chat* chat = new Chat();
	chat->addNewMessage("psivori", "hola");
	chat->addNewMessage("jferrio", "chau");
	EXPECT_CALL(*mockChatDao, get("jferrio-psivori")).Times(AtLeast(1)).WillOnce(Return(chat));
	ChatService* chatService = new ChatService(mockChatDao);
	EXPECT_CALL(*mockChatDao, put("jferrio-psivori", _)).Times(1);
	EXPECT_NO_THROW({chatService->addNewMessage("psivori","jferrio", "hola como estas");});
	delete chatService;
}

/**
 * Test para probar el caso de que no exista el chat buscado
 */
TEST(ChatServiceAddNewMessageNonexistentChatTest,addNewMessageNonexistentChat){
	MockChatDao* mockChatDao = new MockChatDao();
	EXPECT_CALL(*mockChatDao, get("alinari-psivori")).Times(1).WillRepeatedly(ThrowEntityNotFoundException());

	ChatService* chatService = new ChatService(mockChatDao);
	try{
		chatService->addNewMessage("alinari", "psivori", "no existis");
		FAIL();
	}
	catch(EntityNotFoundException& e){
		//OK;
	}
	catch(exception& e){
		FAIL();
	}
	delete chatService;
}


/**
 * Test para probar el camino feliz cuando se actualiza el ultimo mensaje visto por un usuario
 */
TEST(ChatServiceUpdateLastMessageTest,updateLastMessage){
	MockChatDao* mockChatDao = new MockChatDao();

	Chat* chat = new Chat();
	chat->addNewMessage("psivori", "hola");
	chat->addNewMessage("jferrio", "chau");
	chat->setLastSeenByUser1(1);
	chat->setLastSeenByUser2(0);
	EXPECT_CALL(*mockChatDao, get("jferrio-psivori")).Times(AtLeast(1)).WillOnce(Return(chat));
	ChatService* chatService = new ChatService(mockChatDao);
	EXPECT_CALL(*mockChatDao, put("jferrio-psivori", _)).Times(1);
	EXPECT_NO_THROW({chatService->updateLastMessageSeen("psivori","jferrio", 2);});
	delete chatService;
}

/**
 * Test para probar el caso de que no exista el chat buscado
 */
TEST(ChatServiceUpdateLastMessageNonexistentChatTest,updateLastMessageNonexistentChat){
	MockChatDao* mockChatDao = new MockChatDao();
	EXPECT_CALL(*mockChatDao, get("alinari-psivori")).Times(1).WillRepeatedly(ThrowEntityNotFoundException());

	ChatService* chatService = new ChatService(mockChatDao);
	try{
		chatService->updateLastMessageSeen("alinari", "psivori", 2);
		FAIL();
	}
	catch(EntityNotFoundException& e){
		//OK;
	}
	catch(exception& e){
		FAIL();
	}
	delete chatService;
}

/**
 * Test para probar el camino feliz cuando se crea un nuevo chat
 */
TEST(ChatServiceCreateChatTest,createChat){
	MockChatDao* mockChatDao = new MockChatDao();

	EXPECT_CALL(*mockChatDao, get("jferrio-psivori")).Times(AtLeast(1)).WillOnce(ThrowEntityNotFoundException());
	ChatService* chatService = new ChatService(mockChatDao);
	EXPECT_CALL(*mockChatDao, put("jferrio-psivori", _)).Times(1);
	EXPECT_NO_THROW({chatService->createChat("psivori","jferrio");});
	delete chatService;
}

/**
 * Test para probar el caso de que ya exista el chat a crear
 */
TEST(ChatServiceCreateExistentChatTest,createExistentChat){
	MockChatDao* mockChatDao = new MockChatDao();

	ChatService* chatService = new ChatService(mockChatDao);
	try{
		chatService->createChat("jferrio", "psivori");
		FAIL();
	}
	catch(EntityExistsException& e){
		//OK;
	}
	catch(exception& e){
		FAIL();
	}
	delete chatService;
}


/**
 * Test para probar el camino feliz cuando se busca un chat
 */
TEST(ChatServiceGetChatTest,getChatTest){
	MockChatDao* mockChatDao = new MockChatDao();

	Chat* chat = new Chat();
	chat->addNewMessage("psivori", "hola");
	chat->addNewMessage("jferrio", "chau");
	chat->setLastSeenByUser1(1);
	chat->setLastSeenByUser2(0);
	EXPECT_CALL(*mockChatDao, get("jferrio-psivori")).Times(AtLeast(1)).WillOnce(Return(chat));
	ChatService* chatService = new ChatService(mockChatDao);
	Chat* chat2;
	EXPECT_NO_THROW({chat2 = chatService->getChat("psivori","jferrio");});
	ASSERT_TRUE(chat==chat2);
	delete chatService;
	delete chat;
}

/**
 * Test para probar el caso de que no exista el chat buscado
 */
TEST(ChatServiceGetNonexistentChatTest,updateLastMessageNonexistentChat){
	MockChatDao* mockChatDao = new MockChatDao();
	EXPECT_CALL(*mockChatDao, get("alinari-psivori")).Times(1).WillRepeatedly(ThrowEntityNotFoundException());

	ChatService* chatService = new ChatService(mockChatDao);
	try{
		chatService->getChat("psivori", "alinari");
		FAIL();
	}
	catch(EntityNotFoundException& e){
		//OK;
	}
	catch(exception& e){
		FAIL();
	}
	delete chatService;
}


/*
 * Correr con valgrind: valgrind --leak-check=full -v ./mockMatchServiceTest
 */
int main(int argc, char* argv[]){
	::testing::InitGoogleMock(&argc, argv);

	initialize();
	BasicConfigurator config;
	config.configure();

	Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("main"));
	LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Iniciando Tests"));

	return RUN_ALL_TESTS();
}



