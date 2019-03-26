//
// Author: JMAC
// ~~~~~~~~~~~~
//

#include "proxy_handler.h"

std::unique_ptr<RequestHandler> ProxyHandler::create(const ConfigOptions* options) {
  std::unique_ptr<RequestHandler> proxy_handler(new ProxyHandler(options));
  return proxy_handler;
}

std::unique_ptr<Reply> ProxyHandler::HandleRequest(const Request& http_request) {
  std::string destination = options->path_proxy.at(http_request.url_root);
  std::string request_query = http_request.url_path;
  std::unique_ptr<Reply> http_response = this->Redirect(destination, request_query);
  return http_response;
}

std::unique_ptr<Reply> ProxyHandler::Redirect(std::string destination, std::string request_query) {
  std::unique_ptr<Reply> http_response(new Reply());

  // parse destination for IP address and port number if given
  std::string portNum = "80";
  int indexOfColon = destination.find(":");
  if (indexOfColon != -1) {
    portNum = destination.substr(indexOfColon + 1);
    destination = destination.substr(0, indexOfColon);
  }
  BOOST_LOG_TRIVIAL(info) << "ProxyHandler::Redirect->Destination = " << destination << ", Port Number = " << portNum;

  // adapted example from https://stackoverflow.com/questions/28728347/sending-http-get-request-using-boostasio-similar-to-curl
  try {
    boost::asio::io_service io_service;

    tcp::resolver resolver(io_service);
    tcp::resolver::query query(destination.c_str(), portNum);
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    tcp::socket socket(io_service);
    boost::asio::connect(socket, endpoint_iterator);

    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream << "GET " << request_query.c_str() << " HTTP/1.1\r\n";
    request_stream << "Host: " << destination.c_str() << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";

    boost::asio::write(socket, request);
    boost::asio::streambuf response;

    // read status line
    boost::asio::read_until(socket, response, "\r\n");
    std::istream response_stream(&response);
    std::string http_version;
    response_stream >> http_version;
    unsigned int status_code;
    response_stream >> status_code;
    std::string status_message;
    std::getline(response_stream, status_message);

    // read headers
    boost::asio::read_until(socket, response, "\r\n\r\n");
    std::string header;
    std::string header_buf;

    while (std::getline(response_stream, header) && header != "\r") {
      header_buf += header + "\r\n";
      if (status_code / 100 == 3) {
        if (header.find("Location: ") != std::string::npos) {
          std::string location = header.substr(10, std::string::npos);
          std::string url = location.substr(location.find("://") + 3);
          int split_index = url.find("/");
          std::string dest = url.substr(0, split_index);
          std::string req_query = url.substr(split_index);
          return Redirect(dest, req_query);
        }
      }
    }

    std::ostringstream response_content;
    if (response.size() > 0) {
      response_content << &response;
    }

    boost::system::error_code error;
    while (boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error)) {
      response_content << &response;
    }

    if (error != boost::asio::error::eof) {
      throw boost::system::system_error(error);
    }

    http_response->status_code = status_code;
    http_response->headers = header_buf;
    http_response->content = response_content.str();
    http_response->headers += RequestHandlerUtil::GetContentLength(http_response->content);
    BOOST_LOG_TRIVIAL(info) << "ProxyHandler::Redirect-> response_code: " \
                            << http_response->status_code << ", " \
                            << http_response->mime_type << ", " \
                            << http_response->headers;
    return http_response;
  } catch (std::exception& e) {
    http_response->status_code = 500;
    http_response->content = "Error serving proxy content!";
    return http_response;
  }
}
