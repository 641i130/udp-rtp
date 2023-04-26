#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char **argv) {
  // Start the program!
  std::string addr = "127.0.0.1";
  int port = 33333;
  bool type = false; // False is server
  // Parse cli flags
  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];
    if (arg == "-a" || arg == "--multicast-address") {
      if (i + 1 < argc)
        // Get the next argument and set it to the addr var
        addr = argv[++i];
    } else if (arg == "-p" || arg == "--port") {
      if (i + 1 < argc) {
        // Get the next argument and set it to the port var
        port = std::atoi(argv[++i]);
      }
    } else if (arg == "-s" || arg == "--server" || arg == "-c" ||
               arg == "--client") {
      if (arg == "-c" || arg == "--client") {
        type = true;
      }
    } else if (arg == "-h" || arg == "--help") {
      std::cout
          << "Usage: " << argv[0] << " [OPTIONS]\n"
          << "  -s, --server      Server mode\n"
          << "  -c, --client      Client mode\n"
          << "  -p, --port        Port number to use (default: 12345)\n"
          << "  -a, --address     Multicast group address to use (default: "
             "239.0.0.1)\n"
          << "  -h, --help        Display this help page\n";
      std::exit(0);
    } else {
      std::cerr << "Unrecognized argument: " << arg << std::endl;
      std::cout
          << "Usage: " << argv[0] << " [OPTIONS]\n"
          << "  -p, --port        Port number to use (default: 12345)\n"
          << "  -a, --address     Multicast group address to use (default: "
             "239.0.0.1)\n"
          << "  -h, --help        Display this help page\n";
      std::exit(1);
    }
  }
  // Print arguments
  std::cout << "The multicast-address is set to: ";
  std::cout << addr << ":" << port << "\n";

  // Create the multicast server!!!

  // First, create the UDP socket
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    std::cerr << "Error creating socket! Socket error '" << sockfd << "'"
              << std::endl;
    std::exit(1);
  }

  // Allow the multiple sockets to use the same address
  // From my understanding this means multiple users can connect
  int reuseaddr = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr,
                 sizeof(reuseaddr)) < 0) {
    std::cerr << "Error setting socket options! Socket error '" << reuseaddr
              << "'" << std::endl;
    std::exit(1);
  }
  if (!type) {
    // Bind the socket to the multicast address and port!
    sockaddr_in saddr;
    std::memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = inet_addr(addr.c_str());
    saddr.sin_port = htons(port);
    int bound = bind(sockfd, (sockaddr *)&saddr, sizeof(saddr));
    if (bound < 0) {
      std::cerr << "Error binding socket! Socket error '" << bound << "'"
                << std::endl;
      std::exit(1);
    }

    // Receive and print incoming data
    char buf[1024];
    sockaddr_in client_addr;
    socklen_t addrlen = sizeof(client_addr);
    ssize_t nbytes = recvfrom(sockfd, buf, sizeof(buf), 0,
                              (sockaddr *)&client_addr, &addrlen);
    if (nbytes < 0) {
      std::cerr << "Error receiving data" << std::endl;
      std::exit(1);
    }
    std::cout << "Received " << nbytes << " bytes: " << std::string(buf, nbytes)
              << std::endl;
    // Close the socket
    close(sockfd);

    return 0;
  } else if (type) {
    // Uses ip and port given to connect to
    // Client mode
    // Create a UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
      std::cerr << "Error creating socket! Error code '" << sockfd << "'"
                << std::endl;
      std::exit(1);
    }

    // Resolve host address
    sockaddr_in saddr;
    std::memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    if (inet_pton(AF_INET, addr.c_str(), &saddr.sin_addr) <= 0) {
      std::cerr << "Error resolving host address" << std::endl;
      std::exit(1);
    }

    // Connect to the server
    // Turns out you don't connect
    /*
    int srv = connect(sockfd, (sockaddr *)&addr, sizeof(addr));
    if (srv < 0) {
      std::cerr << "Error connecting to server! Error code '" << srv << "'"
                << std::endl;
      std::exit(1);
    }
    */
    // Send a message to the server
    const char *message = "Hello, server!";
    ssize_t nbytes = sendto(sockfd, message, std::strlen(message), 0,
                            (sockaddr *)&saddr, sizeof(saddr));
    if (nbytes < 0) {
      std::cerr << "Error sending message to server" << std::endl;
      std::exit(1);
    }

    // Receive a response from the server
    char buf[1024];
    nbytes = recv(sockfd, buf, sizeof(buf), 0);
    if (nbytes < 0) {
      std::cerr << "Error receiving message from server" << std::endl;
      std::exit(1);
    }
    std::cout << "Received " << nbytes << " bytes: " << std::string(buf, nbytes)
              << std::endl;

    // Close the socket
    close(sockfd);
    return 0;
  }
}
