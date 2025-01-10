#include <boost/asio.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>
#include <boost/regex.hpp>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

int boost_regex() {
  std::string line;
  boost::regex pat("^Subject: (Re: |Aw: )*(.*)");

  while (std::cin) {
    std::getline(std::cin, line);
    boost::smatch matches;
    if (boost::regex_match(line, matches, pat))
      std::cout << matches[2] << std::endl;
  }

  return 0;
}

int main() {

  constexpr const char *ipAddress = IP_ADDR;

  boost::system::error_code ec;

  boost::asio::io_context context;

  boost::asio::ip::tcp::endpoint end_pt(
      boost::asio::ip::make_address_v4(ipAddress, ec), PORT);

  boost::asio::ip::tcp::socket socket(context);

  socket.connect(end_pt, ec);

  if (!ec) {

    std::cout << "Connected " << std::endl;

  } else {

    std::cout << "Failed because " << ec.message() << std::endl;
  }

  if (socket.is_open()) {

    std::string sRequest = "GET /index.html HTTP/1.1\r\n"
                           "HOST: example.com\r\n"
                           "Connection: close\r\n\r\n";

    socket.write_some(boost::asio::buffer(sRequest.data(), sRequest.size()),
                      ec);

    // std::this_thread::sleep_for(std::chrono::milliseconds(500));
    socket.wait(socket.wait_read);

    std::size_t bytes = socket.available();

    std::cout << "Bytes available: " << bytes << std::endl;

    if (bytes > 0) {

      std::vector<char> vBuff(bytes);
      socket.read_some(boost::asio::buffer(vBuff.data(), vBuff.size()), ec);

      std::cout << "Err mess on reading socket: " << ec.message() << std::endl;

      for (auto byte : vBuff) {
        std::cout << byte;
      }
    }
  }

  return 0;
}