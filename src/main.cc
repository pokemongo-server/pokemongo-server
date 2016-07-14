#include <fstream>
#include <iostream>
#include <iomanip>
#include <ctime>
#include "proto/auth_server.pb.h"
#include "proto/game_packets.pb.h"
#include "pokelist.h"

using namespace std;

void print_timestamp(long int utime) {
    struct tm *info;
    char buf[80];
    info = localtime(&utime);

    strftime(buf, 100, "%Y-%m-%d %H:%M:%S", info);
    cout << buf;
}

int main(int argc, char *argv[])
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    if(argc != 2)
        return -1;

    fstream in (argv[1], ios::in | ios::binary);
    if(!in)
        return -1;
    
    AuthServer c;
    c.ParseFromIstream(&in);


    cout << "[100] size:" << c.game_size() << endl;

    if(c.game_size() != 5) {
        cout << "You must have 5 100 {}\n";
        return -1;
    }

    GameContainer  ppc;
    PlayerParams   ppm;
    TokenContainer tok;
    ppc.ParseFromString(c.game(0));
    ppm.ParseFromString(ppc.params());
    tok.ParseFromString(c.game(4));
    
    cout << ppm.username() << endl;

    cout << tok.infos().version() << endl;

    GamePacket pkt;
    ppc.ParseFromString(c.game(2));
    pkt.ParseFromString(ppc.params());

    for(int i = 0; i < pkt.state_size(); i++) {
        State s;
        s.ParseFromString(pkt.state(i));
        cout << "Parsing state " << setw(3) << i << " ";
        if(s.timestamp() > 0) {
            cout << "(";
            cout << s.timestamp();
//            print_timestamp(s.timestamp() / 1000);
            cout << ") ";
        } else {
            cout << "                ";
        //    cout << "      ";
        }

        // 3 -> 1
        if(s.data().has_p()) {
            const Pokemon& p = s.data().p();
            cout << "[POKE] ";
            if(p.has_pokedex_id()) {
                int pokeid = p.pokedex_id();
                const char *pokename = "Unk";
                if(pokeid >=0 && pokeid <= 151)
                    pokename = pokemon_names[pokeid];
                cout << pokename << "(" << pokeid << ") ";
            } else {
                if(p.is_egg()) {
                    cout << "EGG";
                } else 
                    cout << "??";
            }
        }

        // 3 -> 2
        else if(s.data().has_u2()) {
            const Unknown2& u2 = s.data().u2();
            cout << "[UNK2] " << u2.unk1() << " " << u2.unk2();
        }

        // 3 -> 3
        else if(s.data().has_u3()) {
            const Unknown3& u3 = s.data().u3();
            cout << "[UNK3] " << u3.maybe_pokedex_id() << " " << u3.unk2() << " " << u3.unk3();
        }

        // 3 -> 10
        else if(s.data().has_u10()) {
            const Unknown10& u10 = s.data().u10();
            cout << "[UN10] " << u10.maybe_pokedex_id() << " " << u10.unk2();
        }

        else if(s.data().has_incubator()) {
            const EggIncubatorList& egg = s.data().incubator();  
            cout << "[EGGI] Egg incubator list";
        } else {
            cout << "???????????????";
        }

        cout << endl;
    }

    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}
