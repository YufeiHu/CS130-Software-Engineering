<!-- https://gist.github.com/PurpleBooth/109311bb0361f32d87a2 -->
# Nuke-server 

One simple web server developed written in C++ by Yufei Hu, Zhechen Xu, Junheng Hao, and Qi Zeng for the UCLA CS130 Software Engineering class.

## Abstract

This contributor file serves as a helper documentation which will walk through the basic layout of our source code, how to deploy the project on both your local machine and Google Cloud Platform, and a detailed instruction on how to add a new request handler to the server.

## Prerequisites

Before making any changes, make sure you meet all the prerequisites needed for the server. You may check the formal CS130 setup instructions below:

* [Developemnt Environment](https://docs.google.com/document/d/1mL83U4DXll9oe-Vm4g3vbvLD6NoLxkfQ_J0zps6c_tY/edit#heading=h.lgmhii7a2jv1) - The CS130 Development Environment Setup

<!-- ## Installation

A step by step series of examples that tell you how to get a development env running

Say what the step will be

```
Give the example
```

And repeat

```
until finished
```

End with an example of getting some data out of the system or using it for a little demo -->

## Source Code Layout

The main layout of our server's source code files is shown below:

```
src/
    main.cc
    config_parser.cc
    request_handler_manager.cc
    request_handler_util.cc
    request_handler.cc
    bad_request_handler.cc
    not_found_handler.cc
    echo_handler.cc
    file_handler.cc
    markdown.cc
    markdown_tokens.cc
    server.cc
    server_status.cc
    status_handler.cc
    session.cc
include/
test/
docker/
cmake/
static/
    app/
    img/
    text/
CMakeLists.txt
rebuild.sh
rebuild_test.sh
server_config
README.md
```

## Running the tests

A test shell file has been added which could help you run both trival test and coverage test by simply run:

```
$ ./rebuild.sh
```
This will give you all the results for the test and coverage test. For the coverage report, you could find that in the ```build_coverage``` folder.

### Break down into rebuild.sh

We first remove any legacy ```build``` or ```build_coverage``` files, and then create ```build``` or ```build_coverage``` folder. After that the shell runs the following commands:

```
$ mkdir build
$ cd build
$ cmake ..
$ make
```

or for the coverage test:

```
$ mkdir build_coverage
$ cd build_coverage
$ cmake -DCMAKE_BUILD_TYPE=Coverage ..
$ make coverage
```

## Running the server

Similarly, just run the shell:

```
$ ./rebuild_test.sh
```

### Break down into rebuild_test.sh

The only difference between ```rebuild_test.sh``` and ```rebuild.sh``` is that after running the make command in the build folder the ```rebuild_test.sh``` runs another line of command:

```
$ ./bin/nuke_server ../server_config_test
```

<!-- ## Deployment

Add additional notes about how to deploy this on a live system

## Built With

* [Dropwizard](http://www.dropwizard.io/1.0.2/docs/) - The web framework used
* [Maven](https://maven.apache.org/) - Dependency Management
* [ROME](https://rometools.github.io/rome/) - Used to generate RSS Feeds
 -->

## Contributing

### Logic of the request handler
When the server starts a new session and passes the content of the configuration file to the newly started session, the session will then follow 5 steps listed below:
* The session will start listening to the HTTP request input by the user. Once two consecutive Windows new line symbols ("\r\n\r\n") are captured, the session will then consider the current user input as one complete HTTP request.
* The session then call its member function ParseRequest to split the HTTP request into method, URL, and protocol.
* The session will then call its member function ParseUrl to further split the URL into 3 components: URL type (e.g. echo/img/text/app), URL path (e.g. index.html), and URL extension (e.g. html)
* The session then calls its member function GetResponse to tell if the current HTTP request is legal or not depending on its request method (has to be GET), request protocol (has to be HTTP/1.1), URL type (has to be a legal value and not empty), URL extension (has to be not empty) and etc.
* If the HTTP request is legal, the session will finally initialize either an echo_handler class object or file_handler class object to generate an HTTP response based on the given legal HTTP request.

### Adding a handler
In order to add a new handler, you need to first create both ```new_request_handler.h``` and ```new_request_handler.cc``` file in ```include``` folder and ```src``` folder respectively. Then, you should give the class definition and its implementation for the new handler in the ```new_request_handler.h``` and ```new_request_handler.cc``` separately. We also prepared a ```request_handler_util.cc``` for some uitilies you may find useful.

#### An example of an existing handler
Here we will give you a detailed example of the existing ```echo_handler```. The implementation of how the ```echo_handler``` handlers request is shown below:
```
#include <iostream>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include "echo_handler.h"

std::unique_ptr<RequestHandler> EchoHandler::create(const ConfigOptions* options) {
  std::unique_ptr<RequestHandler> echo_handler(new EchoHandler(options));
  return echo_handler;
}

std::unique_ptr<Reply> EchoHandler::HandleRequest(const Request& http_request) {
  std::unique_ptr<Reply> http_response(new Reply());
  http_response->status_code = 200;
  http_response->mime_type = "html";
  http_response->headers = "Content-Type: text/html\r\n";
  http_response->content = http_request.all;
  http_response->headers += RequestHandlerUtil::GetContentLength(http_response->content);
  BOOST_LOG_TRIVIAL(info) << "Response: " << http_response->status_code \
                             << http_response->mime_type \
                             + http_response->headers \
                             + http_response->content << "\n";
  return http_response;
}
```

Basically, the response has the following 5 parts:
* request status code
* request mime type
* request headers
* request content
* request headers

One handler template might be helpful for adding a new handler is attached:
First, include all the header files you need for the implementation.
```
#include <xxx>
#include <xxx/xxx.hpp>
#include "xxx.h"
```

Then, replace the ```NewHandler``` and ```new_handler``` below with your desired handler design.
```
std::unique_ptr<RequestHandler> NewHandler::create(const ConfigOptions* options) {
  std::unique_ptr<RequestHandler> new_handler(new NewHandler(options));
  return new_handler;
}
```

Finally, write the implementation code for your handler.
```
std::unique_ptr<Reply> NewHandler::HandleRequest(const Request& http_request) {
  std::unique_ptr<Reply> http_response(new Reply());
  // your code here
  return http_response;
}
```

You don't have to follow the template above, but it may ease your pain adding a new handler.

#### Header files
All the header files are listed below:
```
include/
    config_options.h
    config_parser.h
    request_handler_manager.h
    request_handler_util.h
    request_handler.h
    bad_request_handler.h
    not_found_handler.h
    echo_handler.h
    file_handler.h
    markdown.h
    markdown_tokens.h
    server.h
    server_status.h
    status_handler.h
    session.h
```

Each handler has its own individual header file, and we also have a ```request_handler.h``` which should be included into each of the handler you add. The ```request_handler.h``` includes almost everything you need to add a new handler's header file, and it also gives the definition of the request handler class itself.

```
#ifndef REQUEST_HANDLER
#define REQUEST_HANDLER

#include <map>
#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include "markdown.h"
#include "request_handler_util.h"
#include "config_options.h"
using boost::asio::ip::tcp;

class RequestHandler {
 public:
  RequestHandler(const ConfigOptions* options_upstream) : options(options_upstream) { }
  const ConfigOptions* options;
  virtual ~RequestHandler() {}
  virtual std::unique_ptr<Reply> HandleRequest(const Request& http_request) = 0;
};

#endif
```

Here, we also offer a header file template for your convenience if you want to add a new feature:
First, replace ```NEW_HANDLER``` with your design and, again, don't forget to include ```request_handler.h```.
```
#ifndef NEW_HANDLER
#define NEW_HANDLER

#include "request_handler.h"
```

After that, fill in the body with your handler.
```
class NewHandler : public RequestHandler {
 public:
  static std::unique_ptr<RequestHandler> create(const ConfigOptions* options);
  NewHandler(const ConfigOptions* options_upstream) : RequestHandler(options_upstream) { }
  std::unique_ptr<Reply> HandleRequest(const Request& http_request);
};

#endif
```


### API Report
Please read [Nuke-server API Written Report](https://docs.google.com/document/d/1BzaB-oCRyJtqDNsAn2acnw3QqPl3osi6njLb4GL4IIk/edit?usp=sharing) for more details.


## Authors

* **Yufei Hu** 
* **Zhechen Xu** 
* **Junheng Hao** 
* **Qi Zeng** 

<!-- ## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Hat tip to anyone whose code was used
* Inspiration
* etc -->
