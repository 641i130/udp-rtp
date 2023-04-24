#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <unistd.h>

int main(int argc, char **argv) {
  // Start the program!
  std::string addr = "239.0.0.1";
  int port = 33333;

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
    } else if (arg == "-h" || arg == "--help") {
      std::cout
          << "Usage: " << argv[0] << " [OPTIONS]\n"
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
  std::cout << addr << ":" << port;

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

  // Bind the socket to the multicast address and port!
}
