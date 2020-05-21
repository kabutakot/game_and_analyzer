#pragma once
#include <string>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <vector>


class TcpMarketDataServer
{
	enum class MessageType : uint16_t {AUCTION_INFO, PLAYER_INFO, ACCEPT, ERROR};
	enum class CommandType : uint16_t {AUCTION_MATERIALS, AUCTION_PROD, PRODUCE, BUILD, END_TURN};

#pragma pack(push, 1)
	struct DataHeader
	{
		MessageType type; //DATA
		uint16_t length;
	};
#pragma pack(pop)

	//server
	static const int MAX_EPOLL_EVENTS = 100;
	static const int BACK_LOG = 100;
	static const int PAYLOAD_SIZE = 1540;

	std::string _bind_address;
	int _bind_port;
	int _listen_fd;
	std::vector <int> _accepted_fd;
	std::string _message_remainder;
	int _epoll_fd;

	ServerState _current_state = ServerState::WAITING;


	void SetNonBlocking (int fd);
	void ShutDown();
	void ReadSocket(int fd);
	void ParseMessage(int fd, std::string& message);
	void ParseData(int fd, const char* data, uint16_t length);
	void ExecuteCommand(int fd, CommandType command);
	void CopyIntoArray(char **dest, const void *source, unsigned bytes);
	void PrintPacket (const char * packet, int size);

public:
	TcpMarketDataServer();
	void Init(std::string& addr, int port);
	void SendMessage(int fd, std::string& msg);
	void Run();
	~TcpMarketDataServer();
};

