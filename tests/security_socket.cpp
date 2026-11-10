#include <cassert>
#include <cstring>
#include <string>
#include <thread>
#include <vector>
#define UNIX
#define __PLINK_H__
#define __HELPER_H__
#define __OPTIONS_H__
using namespace std;
class Plink { public: void printLOG(std::string) {} };
#include "../sockets.cpp"

static void check(const char *response, const std::vector<std::string>& expected) {
    int listener=socket(AF_INET, SOCK_STREAM, 0);
    assert(listener >= 0);
    sockaddr_in address;
    std::memset(&address, 0, sizeof(address));
    address.sin_family=AF_INET;
    address.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
    assert(bind(listener, (sockaddr*)&address, sizeof(address))==0);
    socklen_t length=sizeof(address);
    assert(getsockname(listener, (sockaddr*)&address, &length)==0);
    assert(listen(listener, 1)==0);
    std::thread peer([=]() {
        int client=accept(listener, NULL, NULL);
        assert(client >= 0);
        char request[128];
        assert(recv(client, request, sizeof(request), 0)>0);
        if (response) {
            size_t bytes=std::strlen(response);
            assert(send(client, response, bytes, 0)==(ssize_t)bytes);
        }
        close(client);
        close(listener);
    });
    Plink logging_stub;
    assert(socketConnection(&logging_stub, "127.0.0.1", ntohs(address.sin_port), "request")==expected);
    peer.join();
}
int main() {
    check(NULL, {});
    check("one two", {"one", "two"});
    puts("PASS socket peer EOF before data and after partial data");
}
