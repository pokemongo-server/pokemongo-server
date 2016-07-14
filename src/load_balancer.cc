#include <cstdio>
#include <iostream>
#include <cstring>
#include "fcgio.h"
#include "proto/auth_server.pb.h"
#include "proto/auth_client.pb.h"

using namespace std;

void load_balance() {
    AuthClient c;
    AuthServer s;
    if(!c.ParseFromIstream(&cin)) {
        cout << "Invalid data." << endl;
        return;
    }
    
    s.set_type(53); // REDIRECT
    s.set_uid(c.rpc_id());
    printf("Auth user: %lu\n", c.rpc_id());
    s.set_redirect_to("pgorelease.nianticlabs.com/plfe/101");

    AuthCookie a;
    a.set_token1("LOLNOPE");
    a.set_timestamp(time(NULL) * 1000);
    a.set_token2("BUAHAHAHAH");

    s.set_allocated_cookie(&a);
    s.add_game("");
    s.add_game("");

    cout << s.SerializeAsString();
    s.release_cookie();
}

int main(void) {

    GOOGLE_PROTOBUF_VERIFY_VERSION;
    // Backup the stdio streambufs
    streambuf * cin_streambuf  = cin.rdbuf();
    streambuf * cout_streambuf = cout.rdbuf();
    streambuf * cerr_streambuf = cerr.rdbuf();

    FCGX_Request request;

    FCGX_Init();
    FCGX_InitRequest(&request, 0, 0);

    while (FCGX_Accept_r(&request) == 0) {
        fcgi_streambuf cin_fcgi_streambuf(request.in);
        fcgi_streambuf cout_fcgi_streambuf(request.out);
        fcgi_streambuf cerr_fcgi_streambuf(request.err);

        cin.rdbuf(&cin_fcgi_streambuf);
        cout.rdbuf(&cout_fcgi_streambuf);
        cerr.rdbuf(&cerr_fcgi_streambuf);

        char *method = FCGX_GetParam("REQUEST_METHOD", request.envp);

        if(method == NULL || strcmp(method, "POST")) {
            cout << "Content-type: text/html\r\n"
                 << "\r\n"
                 << "DUDE PLZ USE POST HERE.\n";
        } else {
            cout << "\r\n";
            load_balance();
        }

        // Note: the fcgi_streambuf destructor will auto flush
    }

    // restore stdio streambufs
    cin.rdbuf(cin_streambuf);
    cout.rdbuf(cout_streambuf);
    cerr.rdbuf(cerr_streambuf);

    return 0;
}
