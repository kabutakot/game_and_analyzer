#pragma once
#include <string>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <vector>


class TcpMarketDataServer
{
	enum class MessageType : uint16_t {DATA, DATA_FPGA, DATA_FPGA_FINISH, DATA_FPGA_EMPTY, COMMAND, COMMAND_ARRAY};
	enum class CommandType : uint16_t {STAND_BY, STORE_TCP, RELEASE_UDP, MIRROR_TCP, RELEASE_FPGA_MESS, RELEASE_TCP_MESS};
	enum class ServerState : uint16_t {WAITING, STORING, MIRRORING};

#pragma pack(push, 1)
	struct DataHeader
	{
		MessageType type; //DATA
		uint16_t length;
	};
	struct CommandHeader
	{
		MessageType type; //COMMAND
	};
	struct CommandArrayHeader
	{
		MessageType type; //COMMAND_ARRAY
		uint16_t array_length;
	};
#pragma pack(pop)

	//server
	static const int MAX_EPOLL_EVENTS = 100;
	static const int BACK_LOG = 100;
	static const int PAYLOAD_SIZE = 1540;
	const std::string kSendMsg = "Send";
	const std::string kEndMsg = "End";
	const std::string kStartFix = "8=FIX.4.4";
	struct
	{
		uint16_t blockLength;
		uint16_t templateId;
		uint16_t schemaId;
		uint16_t version;
	} const kStartTwimeNew = {46, 6000, 19781, 2}, kStartTwimeReplace = {41, 6003, 19781, 2};

	std::string _bind_address;
	int _bind_port;
	int _listen_fd, _main_fd = -1;
	std::vector <int> _auxiliary_fd;
	std::string _message_remainder;
	int _epoll_fd;

	std::vector<std::string> _stored_packets;
	std::vector<std::string> _fpga_packets;
	int send_count = 0, recv_count = 0;
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

