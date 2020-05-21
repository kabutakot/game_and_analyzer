#include "TcpMarketDataServer.h"
#include <netinet/ip.h>
#include <algorithm>
#include <cstring>
#include <unistd.h>


#define TRY(x)                                                  \
  do {                                                          \
    int __rc = (x);                                             \
    if( __rc < 0 ) {                                            \
      fprintf(stderr, "ERROR: TRY(%s) failed\n", #x);           \
      fprintf(stderr, "ERROR: at %s:%d\n", __FILE__, __LINE__); \
      fprintf(stderr, "ERROR: rc=%d errno=%d (%s)\n",           \
              __rc, errno, strerror(errno));                    \
      abort();                                                  \
    }                                                           \
  } while( 0 )


#define TEST(x)                                                 \
  do {                                                          \
    if( ! (x) ) {                                               \
      fprintf(stderr, "ERROR: TEST(%s) failed\n", #x);          \
      fprintf(stderr, "ERROR: at %s:%d\n", __FILE__, __LINE__); \
      abort();                                                  \
    }                                                           \
  } while( 0 )

void TcpMarketDataServer::SetNonBlocking(int fd)
{
	int opts;
	TRY(opts = fcntl(fd, F_GETFL));
	opts = (opts | O_NONBLOCK);
	TRY(fcntl(fd, F_SETFL, opts) < 0);
}

void TcpMarketDataServer::ShutDown()
{
	for (auto& fd : _accepted_fd)
		close(fd);
	close (_main_fd);
	close (_listen_fd);
}

TcpMarketDataServer::TcpMarketDataServer()
{
}

void TcpMarketDataServer::Init(std::string& addr, int port)
{
	TRY(_listen_fd = socket(AF_INET, SOCK_STREAM, 0));
	int one = 1;
	TRY(setsockopt(_listen_fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)));
	TRY(setsockopt(_listen_fd, SOL_TCP, TCP_NODELAY, &one, sizeof(one)));
	
	sockaddr_in serv_addr;
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = (addr.c_str() == nullptr) ? INADDR_ANY : inet_addr(addr.c_str());
	serv_addr.sin_port = htons(port);
	TRY(bind(_listen_fd, (sockaddr*)(&serv_addr), sizeof(serv_addr)));


	SetNonBlocking(_listen_fd);
	TRY(_epoll_fd = epoll_create(MAX_EPOLL_EVENTS));
	epoll_event listen_ev;
	listen_ev.events = EPOLLIN | EPOLLRDHUP;
	listen_ev.data.fd = _listen_fd;
	TRY(epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, _listen_fd, &listen_ev));

	TRY(listen(_listen_fd, BACK_LOG));
}

void TcpMarketDataServer::RunPreambles(uint64_t limit)
{
	socklen_t client;
	epoll_event events[MAX_EPOLL_EVENTS];
	epoll_event conn_event;
	sockaddr_in client_addr;
	uint64_t send_count = 0;
	while (true)
	{
		int nfds = epoll_wait(_epoll_fd, events, MAX_EPOLL_EVENTS, 5);

		for (int n = 0; n < nfds; ++n)
		{
			if (events[n].data.fd == _listen_fd)
			{
				client = sizeof(client_addr);
				int one;
				int fd;
				TRY(fd = accept(_listen_fd, (struct sockaddr*) &client_addr, &client));
				onload_move_fd(fd);
				TRY(setsockopt(fd, SOL_TCP, TCP_NODELAY, &one, sizeof(one)));

				// Добавление клиентского дескриптора в массив ожидания
				SetNonBlocking(fd);
				conn_event.data.fd = fd;
				conn_event.events = EPOLLIN | EPOLLRDHUP;
				if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, fd, &conn_event) < 0)
				{
					close(_main_fd);
					continue;
				}

				if (_main_fd == -1)
				{
					_main_fd = fd;
				}
				else
				{
					_accepted_fd.push_back(fd);
				}
			}
			// Готов клиентский дескриптор
			else
			{
				// Выполням работу с дескриптором
				int fd = events[n].data.fd;

				if (events[n].events & EPOLLRDHUP)
				{
					std::cout.flush();
					auto it = std::find(_accepted_fd.begin(), _accepted_fd.end(), fd);
					if (it != _accepted_fd.end())
						_accepted_fd.erase(it);
					close(fd);
					return;
				}

				if (events[n].events & EPOLLIN)
				{
					const int size = 7;
					char buf[size];
					memset(buf, 0, size);
					struct msghdr msg;
					struct iovec iov = { buf, size };
					char cmsg_buf[size];
					msg.msg_iov = &iov;
					msg.msg_iovlen = 1;
					msg.msg_namelen = 0;
					msg.msg_control = cmsg_buf;
					msg.msg_controllen = sizeof(cmsg_buf);
					msg.msg_flags = 0; /* work-around for onload bug57094 */
					int rc = recvmsg(fd, &msg, MSG_DONTWAIT);
					//Sstd::cout << (char*)msg.msg_iov->iov_base;

					std::string res = _message_remainder;
					int cum_length = res.size();
					while (rc > 0)
					{
						res.resize(cum_length + rc, 0);
						memcpy(&res[cum_length], buf, rc);
						cum_length += rc;
						rc = recvmsg(fd, &msg, MSG_DONTWAIT);
					}
					//fmt::print("res {}\n", res);
					//fmt::print("res {} send count {}\n", res, send_count++);
					if (res.size() != 0 && res.find("Send") != res.npos)
					{
						//if (send_count % 10000 == 0)
						int alpha_num = 0;
						while (isdigit(res[alpha_num+5]))
						{
							alpha_num++;
						}
						auto amount = std::stoi(res.substr(5, alpha_num));
						auto sent = _udp_sender->SendDump(amount);
						send_count += sent;
						if (sent < amount || send_count >= limit)
						{
							auto str = fmt::format("Finish {}", sent);
							SendMessage(_main_fd, str);
							return;
						}
					}
				}
			}
		}
	}
}

void TcpMarketDataServer::ReadSocket(int fd)
{
	const int size = 2048;
	char buf[size];
	memset(buf, 0, size);
	struct msghdr msg;
	struct iovec iov = { buf, size };
	char cmsg_buf[size];
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_namelen = 0;
	msg.msg_control = cmsg_buf;
	msg.msg_controllen = sizeof(cmsg_buf);
	msg.msg_flags = 0;
	int rc = recvmsg(fd, &msg, MSG_DONTWAIT);
	//Sstd::cout << (char*)msg.msg_iov->iov_base;

	std::string res = _message_remainder;
	int cum_length = res.size();
	while (rc > 0)
	{
		res.resize(cum_length + rc, 0);
		memcpy(&res[cum_length], buf, rc);
		cum_length += rc;
		rc = recvmsg(fd, &msg, MSG_DONTWAIT);
	}

	ParseMessage(fd, res);

}

void TcpMarketDataServer::ParseMessage(int fd, std::string& message)
{
	int size = 0;
	static bool got_first_FPGA_answer = false;
	auto CheckTheSize = [this, &message, &size](uint16_t required_size)
	{
		if (message.size() - size < required_size)
		{
			_message_remainder.resize(message.size() - size);
			memcpy(&_message_remainder[0], &message[size], _message_remainder.size());
			return false;
		}
		return true;
	};
	while (message.size() > size)
	{
		int message_length = 0;
		if (!CheckTheSize(sizeof(MessageType)))
			return;

		MessageType msg_type = static_cast<MessageType>(*(uint16_t*)&message[size]);
		switch (msg_type)
		{
		case MessageType::DATA:
		{
			if (!CheckTheSize(sizeof(DataHeader)))
				return;
			uint16_t length = (*(uint16_t*)&message[sizeof(MessageType) + size]);
			if (!CheckTheSize(sizeof(DataHeader) + length))
				return;

			ParseData(fd, message.c_str() + sizeof(DataHeader) + size, length);
			message_length += sizeof(DataHeader) + length;
			break;
		}
		case MessageType::DATA_FPGA:
		{
			throw std::logic_error("DATA_FPGA type message on server");
		}
		case MessageType::COMMAND:
		{
			if (!CheckTheSize(sizeof(CommandHeader) + sizeof(CommandType)))
				return;

			CommandType command = static_cast<CommandType>(*(uint16_t*)&message[sizeof(CommandHeader) + size]);
			ExecuteCommand(fd, command);
			message_length += sizeof(CommandHeader) + sizeof(CommandType);
			break;
		}
		case MessageType::COMMAND_ARRAY:
		{
			if (!CheckTheSize(sizeof(CommandArrayHeader)))
				return;

			uint16_t array_length = (*(uint16_t*)&message[sizeof(MessageType) + size]);
			if (!CheckTheSize(sizeof(CommandArrayHeader) + array_length * sizeof(CommandType)))
				return;

			message_length += sizeof(CommandArrayHeader);
			for (int i = 0; i < array_length; i++)
			{
				CommandType command = static_cast<CommandType>(*(uint16_t*)&message[sizeof(CommandArrayHeader) + i * sizeof(CommandType) + size]);
				message_length += sizeof(CommandType);
				ExecuteCommand(fd, command);
			}
			break;
		}
		default:
			//if (!got_first_FPGA_answer)
			//{
			//	_got = std::chrono::system_clock::now();
			//	auto micros = std::chrono::time_point_cast<std::chrono::microseconds>(_sent).time_since_epoch().count();
			//	auto micros2 = std::chrono::time_point_cast<std::chrono::microseconds>(_got).time_since_epoch().count();
			//	fmt::print("micros={}\n", micros);
			//	fmt::print("micros2={}\n", micros2);
			//	fmt::print("micros2-micros={}\n", micros2-micros);
			//	//got_first_FPGA_answer = true;
			//}
			// 16 = sizeof("8=FIX.4.4\0019=XXX\001")
			auto max_size = std::max(std::max(sizeof(kStartTwimeNew), sizeof(kStartTwimeReplace)), (decltype(sizeof(kStartTwimeNew)))16);
			if (!CheckTheSize(max_size))
				return;
			//regular tcp mess, check if it's from FPGA
			if(message.find(kStartFix, size) == size)
			{
				//FIX
				auto length_pos = message.find("\0019=", size, 3);
				if (length_pos == message.npos)
				{
					throw std::logic_error("Bad FIX! No length tag");
				}

				length_pos += 3;
				auto length_pos_end = message.find("\001", length_pos, 1);
				if (length_pos_end == message.npos)
				{
					throw std::logic_error("Bad FIX! No length tag end");
				}

				message_length = std::stoi(message.substr(length_pos, length_pos_end - length_pos)) + 23; //magic appears
				assert(message_length <= 999);
				if (!CheckTheSize(message_length))
					return;

				std::string to_push(message_length, 0);
				memcpy(&to_push[0], &message[size], message_length);
				_fpga_packets.push_back(to_push);
			}
			else if (memcmp(&message[size], &kStartTwimeNew, sizeof(kStartTwimeNew)) == 0)
			{
				message_length = kStartTwimeNew.blockLength + 8; // because apparently first 4 uint16_t doesn't count in blockLength
				if (!CheckTheSize(message_length))
					return;
				std::string to_push(message_length, 0);
				memcpy(&to_push[0], &message[size], message_length);
				_fpga_packets.push_back(to_push);
			}
			else if (memcmp(&message[size], &kStartTwimeReplace, sizeof(kStartTwimeReplace)) == 0)
			{
				message_length = kStartTwimeReplace.blockLength + 8; // because apparently first 4 uint16_t doesn't count in blockLength
				if (!CheckTheSize(message_length))
					return;
				std::string to_push(message_length, 0);
				memcpy(&to_push[0], &message[size], message_length);
				_fpga_packets.push_back(to_push);
			}
			else
				throw std::logic_error("Unknown message_type");

			//message_length += message.size();
			////for (int i = 0; i < message.size(); i++)
			////{
			////	if (message[i] == '\001')
			////		message[i] = ' ';
			////}
			//auto to_print = StringHelper::DumpToHex(&message[0], message.size());
			//fmt::print("{}\n", message);
			//fmt::print("{}\n", to_print);
			//std::cout.flush();
			////
		}
		size += message_length;
	}
	_message_remainder.clear();
}

void TcpMarketDataServer::ParseData(int fd, const char* data, uint16_t length)
{
	switch (_current_state)
	{
	case ServerState::WAITING:
		break;
	case ServerState::STORING:
		{
		std::string to_store(length, 0);
		memcpy(&to_store[0], data, length);
		_stored_packets.push_back(to_store);
		//auto to_print = StringHelper::DumpToHex(&_stored_packets[_stored_packets.size() - 1][0], _stored_packets[_stored_packets.size() - 1].size());
		//fmt::print("Packet {} {} - {}\n", _stored_packets[_stored_packets.size() - 1].size(), to_print.size(), to_print);
		assert(_stored_packets[_stored_packets.size() - 1].size() == length);
		break;
		}
	case ServerState::MIRRORING:
		{
		std::string to_send (length + sizeof(DataHeader), 0);
		DataHeader header = {MessageType::DATA, length};
		memcpy(&to_send[0], &header, sizeof(DataHeader));
		memcpy(&to_send[sizeof(DataHeader)], data, length);
		SendMessage(_main_fd, to_send);
		break;
		}
	}
}

void TcpMarketDataServer::ExecuteCommand(int fd, CommandType command)
{
	switch (command)
	{
	case CommandType::STAND_BY:
		{
		_current_state = ServerState::WAITING;
		break;
		}
	case CommandType::STORE_TCP:
		{
		_current_state = ServerState::STORING;
		break;
		}
	case CommandType::RELEASE_UDP:
		{
			_sent = std::chrono::system_clock::now();
			_udp_sender->SendUdpPackets(_stored_packets);
			_stored_packets.clear();
			break;
		}
	case CommandType::MIRROR_TCP:
		{
		_current_state = ServerState::MIRRORING;
		break;
		}
	case CommandType::RELEASE_FPGA_MESS:
	{
		fmt::print("Releasing FPGA mess\n");
		if (_fpga_packets.size() == 0)
		{
			std::string to_send (sizeof(DataHeader), 0);
			DataHeader header = { MessageType::DATA_FPGA_EMPTY, 0 };
			memcpy(&to_send[0], &header, sizeof(DataHeader));
			SendMessage(_main_fd, to_send);
		}
		for (auto i = 0; i < _fpga_packets.size(); i++)
		{
			auto size = _fpga_packets[i].size();
			std::string to_send (size + sizeof(DataHeader), 0);
			DataHeader header = { (i == _fpga_packets.size()-1) ? MessageType::DATA_FPGA_FINISH : MessageType::DATA_FPGA, size};
			memcpy(&to_send[0], &header, sizeof(DataHeader));
			memcpy(&to_send[sizeof(DataHeader)], &_fpga_packets[i][0], _fpga_packets[i].size());
			SendMessage(_main_fd, to_send);
		}
		
		_fpga_packets.clear();
		break;
	}
	case CommandType::RELEASE_TCP_MESS:
	{
		for (auto& packet : _stored_packets)
			SendMessage(_accepted_fd[0], packet);
		_stored_packets.clear();
		break;
	}
	}
}

void TcpMarketDataServer::CopyIntoArray(char** dest, const void* source, unsigned bytes)
{
	memcpy(*dest, source, bytes);
	*dest += bytes;
}

void TcpMarketDataServer::PrintPacket(const char* packet, int size)
{
	fmt::print("size - {}\n", size);
	uint16_t *intBuf = (uint16_t*)packet;
	for (int i = 0; i < size / 2; i++)
	{
		auto stringBuf = fmt::format("{:04x} ", htons(intBuf[i]));
		std::cout << stringBuf;
	}
	if (size % 2)
	{
		fmt::print("{:02x}", htons(*(uint16_t*)(packet + size - 1)) >> 8);
	}
	fmt::print("\n");
}

void TcpMarketDataServer::SendMessage(int fd, std::string& msg)
{
	int rc = send(fd, msg.c_str(), msg.size(), 0);
	if (rc < msg.size() )
	{
		fmt::print("Failed receive rc={} msg_size={}\n", rc, msg.size());
	}
}

void TcpMarketDataServer::Run()
{
	socklen_t client;
	epoll_event events[MAX_EPOLL_EVENTS];
	epoll_event conn_event;
	sockaddr_in client_addr;

	while (true)
	{
		int nfds = epoll_wait(_epoll_fd, events, MAX_EPOLL_EVENTS, 50000);

		for (int n = 0; n < nfds; ++n)
		{
			if (events[n].data.fd == _listen_fd)
			{
				client = sizeof(client_addr);
				int one;
				int fd;
				TRY(fd = accept(_listen_fd, (struct sockaddr*) &client_addr, &client));
				TRY(setsockopt(fd, SOL_TCP, TCP_NODELAY, &one, sizeof(one)));

				// Добавление клиентского дескриптора в массив ожидания
				SetNonBlocking(fd);
				conn_event.data.fd = fd;
				conn_event.events = EPOLLIN | EPOLLRDHUP;
				if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, fd, &conn_event) < 0)
				{
					continue;
				}
			}
			// Готов клиентский дескриптор
			else
			{
				// Выполням работу с дескриптором
				int fd = events[n].data.fd;

				if (events[n].events & EPOLLRDHUP)
				{
					auto it = std::find(_accepted_fd.begin(), _accepted_fd.end(), fd);
					if (it != _accepted_fd.end())
						_accepted_fd.erase(it);
					close(fd);
					return;
				}

				if (events[n].events & EPOLLIN)
				{
					ReadSocket(fd);
				}
			}
		}
	}
}


TcpMarketDataServer::~TcpMarketDataServer()
{
	ShutDown();
}
