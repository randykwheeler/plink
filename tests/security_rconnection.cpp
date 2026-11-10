#include <cassert>
#include <climits>
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>
#include <sys/wait.h>
#include <crypt.h>
#define MAIN
#define SOCK_ERRORS
#include "../Rconnection.cpp"

class TestConnection : public Rconnection {
public:
    void attach(int fd) { s=fd; }
};
class Oversized : public Rexp {
public:
    Oversized() : Rexp(XT_NULL) {}
    Rsize_t storageSize() { return UINT_MAX; }
};

static void read_all(int fd, char *buf, size_t len) {
    while (len) {
        ssize_t n=recv(fd, buf, len, 0);
        assert(n > 0);
        buf+=n;
        len-=n;
    }
}

static void check_assign(size_t n) {
    std::string symbol(n, 's');
    int fds[2];
    assert(socketpair(AF_UNIX, SOCK_STREAM, 0, fds)==0);
    pid_t pid=fork();
    assert(pid >= 0);
    if (!pid) {
        close(fds[0]);
        phdr head;
        read_all(fds[1], (char*)&head, sizeof(head));
        std::vector<char> bytes(ptoi(head.len));
        read_all(fds[1], &bytes[0], bytes.size());
        size_t padded=(n+4)&~(size_t)3;
        assert((ptoi(*(unsigned int*)&bytes[0])>>8)==padded);
        assert(std::memcmp(&bytes[4], symbol.c_str(), n+1)==0);
        for (size_t i=n+1; i<padded; ++i) assert(bytes[4+i]==0);
        assert(bytes.size()==padded+16);
        assert((ptoi(*(unsigned int*)&bytes[4+padded])&255)==DT_SEXP);
        std::memset(&head, 0, sizeof(head));
        head.cmd=itop(RESP_OK);
        assert(send(fds[1], &head, sizeof(head), 0)==sizeof(head));
        close(fds[1]);
        _exit(0);
    }
    close(fds[1]);
    TestConnection connection;
    connection.attach(fds[0]);
    Rexp value(XT_INT, "ABCD", 4);
    assert(connection.assign(symbol.c_str(), &value)==0);
    int status;
    assert(waitpid(pid, &status, 0)==pid);
    assert(WIFEXITED(status) && WEXITSTATUS(status)==0);
}

int main() {
    Rexp empty(XT_NULL);
    assert(empty.data==NULL && empty.next==NULL);
    for (size_t boundary=65536; boundary<=1048576; boundary*=16) {
        for (int offset=-2; offset<=2; ++offset) {
            size_t n=boundary+offset;
            std::string text(n, 't');
            Rmessage message(CMD_eval, text.c_str());
            size_t padded=(n+4)&~(size_t)3;
            assert(message.is_complete());
            assert(message.len==padded+4);
            assert(std::memcmp(message.data+4, text.c_str(), n+1)==0);
            assert((ptoi(*(unsigned int*)message.data)>>8)==padded);
            check_assign(n);
        }
    }
    std::string too_long(0xfffffcU, 'x');
    bool rejected=false;
    try { Rmessage invalid(CMD_eval, too_long.c_str()); }
    catch (const std::length_error&) { rejected=true; }
    assert(rejected);
    TestConnection disconnected;
    Rexp value(XT_INT, "ABCD", 4);
    assert(disconnected.assign(too_long.c_str(), &value)==CERR_not_supported);
    Oversized oversized;
    assert(disconnected.assign("x", &oversized)==CERR_out_of_mem);

    char buf[8];
    std::memset(buf, 'X', sizeof(buf));
    errno=EBADF;
    assert(sockerrorchecks(buf, 4, -1)==-1);
    assert(std::strcmp(buf, "bad")==0 && buf[4]=='X');
    assert(sockerrorchecks(NULL, 0, -1)==-1);
    assert(sockerrorchecks(buf, 0, -1)==-1 && buf[0]=='b');
    errno=99999;
    sockerrorchecks(buf, 4, -1);
    assert(buf[3]==0 && buf[4]=='X');
    puts("PASS Rconnection padding, assignment payloads, overflow rejection and socket error bounds");
}
