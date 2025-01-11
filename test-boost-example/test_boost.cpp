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

std::vector<char> vBuffer(1 * 1024);

void grabSomeData(boost::asio::ip::tcp::socket &socket) {

  socket.async_read_some(boost::asio::buffer(vBuffer.data(), vBuffer.size()),
                         [&](std::error_code ec, std::size_t len) {
                           if (!ec) {
                             std::cout << "\nRead: " << len << "bytes"
                                       << std::endl;
                             for (size_t i = 0; i < len; i++)
                               std::cout << vBuffer[i];

                           } else {
                           }
                           grabSomeData(socket);
                         });
}

int main() {

  constexpr const char *ipAddress = IP_ADDR;

  boost::system::error_code ec;

  // Create a context
  boost::asio::io_context context;

  // Fake tasks context, "idle task"
  // Use executor_work_guard to keep the  io_context running
  auto idleWork = boost::asio::make_work_guard(context);

  // Start context
  std::thread thrContext = std::thread([&]() { context.run(); });

  // create an endpoint
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

    grabSomeData(socket);
    std::string sRequest = "GET /index.html HTTP/1.1\r\n"
                           "HOST: example.com\r\n"
                           "Connection: close\r\n\r\n";

    socket.write_some(boost::asio::buffer(sRequest.data(), sRequest.size()),
                      ec);

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(2000ms);

    context.stop();
    if (thrContext.joinable())
      thrContext.join();
  }

  return 0;
}