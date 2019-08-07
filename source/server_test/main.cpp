#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Util/ServerApplication.h>
#include <iostream>
#include <string>
#include <vector>

using namespace Poco::Net;
using namespace Poco::Util;
using namespace std;

#include "Log.hpp"

class MyRequestHandler : public HTTPRequestHandler
{
public:
    void handleRequest(HTTPServerRequest &req, HTTPServerResponse &resp) override {
        resp.setStatus(HTTPResponse::HTTP_OK);
        resp.setContentType("text/html");
        
        ostream& out = resp.send();
        out << "<h1>Hello world!</h1>"
        << "<p>Count: "  << ++count         << "</p>"
        << "<p>Host: "   << req.getHost()   << "</p>"
        << "<p>Method: " << req.getMethod() << "</p>"
        << "<p>URI: "    << req.getURI()    << "</p>";
        out.flush();
        
        cout << endl
        << "Response sent for count=" << count
        << " and URI=" << req.getURI() << endl;
    }
    
private:
    static int count;
};

int MyRequestHandler::count = 0;

class MyRequestHandlerFactory : public HTTPRequestHandlerFactory
{
public:
    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request) override {
        Log(request.getURI());
        return new MyRequestHandler;
    }
};

class MyServerApp : public ServerApplication {
protected:
    int main(const vector<string>&) {
        HTTPServer server(new MyRequestHandlerFactory, ServerSocket(80), new HTTPServerParams);
        server.start();
        cout << endl << "Server started" << endl;
        
        waitForTerminationRequest();
        
        cout << endl << "Shutting down..." << endl;
        server.stop();
        return Application::EXIT_OK;
    }
};

int main(int argc, char** argv) {
    return MyServerApp().run(argc, argv);
}