#include <cstdio>
#include <iostream>
#include <cstring>
#include "fcgio.h"
#include "proto/game_packets.pb.h"
#include "proto/auth_server.pb.h"
#include "proto/auth_client.pb.h"

using namespace std;

PlayerParams getPlayerParams() {
    PlayerParams p;

    p.set_timestamp(time(NULL) * 1000);
    p.set_username("DAT USER");
    p.set_unk5(1);
    p.set_unk7("\x00\x01\x03\x04\x07");
    p.set_unk8("");
    p.set_unk9(250);
    p.set_unk9(350);
    p.set_unk11("");
    p.set_unk12("");
    p.set_unk13("");

    Quantity *coin = p.add_quantity();
    coin->set_type("POKECOIN");
    coin->set_quantity(1000);

    Quantity *stardust = p.add_quantity();
    stardust->set_type("STARDUST");
    stardust->set_quantity(5000);
    
    return p;
}

GamePacket getPokemonsAndStuff() {
    GamePacket g;
    g.set_timestamp(time(NULL) * 1000);

    // TODO
    return g;
}

TokenContainer getTokenContainer() {
    TokenContainer t;
    TokenContainer_InfoContainer *i = new TokenContainer_InfoContainer();
    TokenContainer_InfoContainer_Something2 *s2 = new TokenContainer_InfoContainer_Something2();
    TokenContainer_InfoContainer_Something3 *s3 = new TokenContainer_InfoContainer_Something3();
    TokenContainer_InfoContainer_Something5 *s5 = new TokenContainer_InfoContainer_Something5();

    s2->set_unk1(40.0); s2->set_unk2(10); s2->set_unk3(1);
    s2->set_unk4(2.0); s2->set_unk6(1000.0);

    s3->set_unk1(100.0);
    s3->set_unk2(200.0);
    s3->set_unk3(50.0);
    s3->set_unk4(5.0);
    s3->set_unk5(30.0);
    s3->set_unk6(10.0);
    s3->set_token("asd lol 123");

    s5->set_unk1(1000);
    s5->set_unk2(1000);
    s5->set_unk3(250);
    s5->set_unk4(350);
    s5->set_unk5(9);

    i->set_allocated_s2(s2);
    i->set_allocated_s3(s3);
    i->set_allocated_s5(s5);
    i->set_version("0.29.0");

    t.set_hash("4a2e9bc330dae60e7b74fc85b98868ab4700802e");
    t.set_allocated_infos(i);

    return t;
}

void serve() {
    AuthClient c;
    AuthServer s;
    if(!c.ParseFromIstream(&cin)) {
        cout << "Invalid data." << endl;
        return;
    }

    printf("Serving: %016lx\n", c.rpc_id());

    AuthCookie a;
    a.set_token1("LOLNOPE");
    a.set_timestamp(time(NULL) * 1000);
    a.set_token2("BUAHAHAHAH");

    int mode = 0;
    for(int i = 0; i < c.info_size(); i++) {
        const AuthInfo &info = c.info(i);
        if((info.id() == 128 || info.id() == 6))
            mode = 4;
        if(info.id() == 7 && info.has_data())
            mode = 2;
        if(info.id() == 300 && info.has_data())
            mode = 3;
        if(info.id() == 4 && info.has_data() && mode == 0)
            mode = 1;
    }

    DaFaqIsThis horror;
    HorrorContainer hc;
    horror.set_unk0(6);
    horror.set_allocated_hc(&hc);

    PlayerParams p;
    GameReady gr;
    GamePacket gp;
    TokenContainer t;

    GameContainer gc;
    GameContainer null_gc;

    s.set_type(1); // AUTH OK
    s.set_uid(c.rpc_id());
    s.set_allocated_horror(&horror);

    gc.set_id(1);
    null_gc.set_id(1);

    t.set_hash("4a2e9bc330dae60e7b74fc85b98868ab4700802e");
    if(mode == 0 || mode == 1) {
        if(mode == 0) {
            p = getPlayerParams();
            gp = getPokemonsAndStuff();
            t = getTokenContainer();
        } else if(mode == 1) {
            p = getPlayerParams();
            gp.set_timestamp(time(NULL) * 1000);
            gp.set_unk_timestamp(time(NULL) * 1000);
        }

        s.set_allocated_cookie(&a);

        gc.set_params(p.SerializeAsString());

        // Player info
        s.add_game(gc.SerializeAsString());

        // Null 100 {}
        s.add_game(null_gc.SerializeAsString());

        // Pokemons & Items
        gc.set_params(gp.SerializeAsString());
        s.add_game(gc.SerializeAsString());

        // Null 100 {}
        s.add_game(null_gc.SerializeAsString());

        // Tokens
        gc.set_params(t.SerializeAsString());
        s.add_game(gc.SerializeAsString());

        cout << s.SerializeAsString();

        s.release_cookie();
    } else if(mode == 2) {
        gr.set_id(1);
        gr.set_ts(time(NULL));
        gr.set_ts_ms((uint64_t)time(NULL) * 1000000);
        gp.set_timestamp(time(NULL) * 1000);
        gp.set_unk_timestamp(time(NULL) * 1000);

        // Player info
        s.add_game(gr.SerializeAsString());

        // Null 100 {}
        s.add_game(null_gc.SerializeAsString());

        // Pokemons & Items
        gc.set_params(gp.SerializeAsString());
        s.add_game(gc.SerializeAsString());

        // Null 100 {}
        s.add_game(null_gc.SerializeAsString());

        // Tokens
        s.add_game(t.SerializeAsString());

        cout << s.SerializeAsString();
    } else if(mode == 3) {  // Now it wants World data
        WorldObjects world;
        world.set_timestamp(time(NULL) * 1000 * 1000);

	// I have no idea 
        WorldObject *w = world.add_object();
        w->set_asset_id("MMH NOPE");
        w->set_data("pm0094");
        w->set_spawn_time((uint64_t)time(NULL) * 1000U * 1000U);
        w->set_unk4(9.5f); 
        w->set_unk5(400121);
        w->set_unk6("");

        gp.set_timestamp(time(NULL) * 1000);
        gp.set_unk_timestamp(time(NULL) * 1000);

        // World info
        s.add_game(world.SerializeAsString());

        // Null 100 {}
        s.add_game(null_gc.SerializeAsString());
        
        // Pokemons & Items
        gc.set_params(gp.SerializeAsString());
        s.add_game(gc.SerializeAsString());
        
        // Null 100 {}
        s.add_game(null_gc.SerializeAsString());
        
        // Tokens
        s.add_game(t.SerializeAsString());

        cout << s.SerializeAsString();
    } else if(mode == 4) {  // Now it wants World data
        gp.set_timestamp(time(NULL) * 1000);
        gp.set_unk_timestamp(time(NULL) * 1000);
        
        gc.set_id(2);
        s.add_game(gc.SerializeAsString());
        s.add_game(null_gc.SerializeAsString());

        gc.set_params(gp.SerializeAsString());
        s.add_game(gc.SerializeAsString());

        s.add_game(null_gc.SerializeAsString());
        s.add_game(t.SerializeAsString());
        null_gc.set_id(2);
        s.add_game(null_gc.SerializeAsString());

        cout << s.SerializeAsString();
    }
    horror.release_hc();
    s.release_horror();
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
            serve();
        }

        // Note: the fcgi_streambuf destructor will auto flush
    }

    // restore stdio streambufs
    cin.rdbuf(cin_streambuf);
    cout.rdbuf(cout_streambuf);
    cerr.rdbuf(cerr_streambuf);

    return 0;
}
