#include "utils.h"
#include "server.cpp"
#include "client.cpp"

/**
 * Funzione utile ad inizializzare e far partire l'esecuzione del server.
 */
void startServer(int argc, char *argv[]) {
    int nport = 9080;
    Port port(nport);
    int thr = 2;
    if (argc >= 2) {
        port = static_cast<uint16_t>(std::stol(argv[1]));
        if (argc == 3)
            thr = std::stoi(argv[2]);
    }
    Address addr(Ipv4::any(), port);
    Server server(addr);
    cout << "Cores = " << hardware_concurrency() << endl;
    cout << "Using " << thr << " threads" << endl;
    cout << "Server started on port: " << port << endl;
    server.init(thr);
    server.start();
}

/**
 * Funzione utile a far partire l'esecuzione del client.
 */
void startClient() {
    Client client;
    client.start();
}

/**
 * Funzione main che prende il server ed il client e li esegue parallelamente su due thread differenti.
 */
int main(int argc, char *argv[]) {
    thread server(startServer, argc, argv), client(startClient);
    server.join();
    client.join();
}
