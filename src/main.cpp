#include <cstdlib>
#include <iostream>

void printHelp() {}

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

  // Create the multicast port server!!!
  //
  //
}
