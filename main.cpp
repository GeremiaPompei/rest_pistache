#include "server.h"
#include "client.h"

using namespace std;

/**
 * Funzione utile ad inizializzare e far partire l'esecuzione del server.
 */
void startServer(bool httpsOn) 
{
    Server server(9080);
    if(httpsOn)
        cout << "Https on!" << endl;
    server.init(2);
    server.start(httpsOn);
}

/**
 * Metodo utile a dividere una stringa in base agli spazi utile per prelevare i dati a riga di comando.
 */
int split(string str, vector<string> *v) 
{
    string token = "";
    for(auto c : str) {
        if(c == ' ') {
            token += "\0";
            v->push_back(token);
            token = "";
        } else {
            token += c;
        }
    }
    token += "\0";
    v->push_back(token);
    for(auto t : *v) if(t=="exit") return 1;
    if(v->size() < 3) return 2;
    return 0;
}

/**
 * Funzione utile a far partire l'esecuzione del client.
 */
void startClient() 
{
    Client client;
    while(true) {
        try {
            cout << "Inserisci dominio di destinazione(http://ip:port/), metodo(POST, GET, DEL), id dell'elemento e in caso di POST nome e descrizione. exit per uscire." << endl;
            cout << "Esempio: http://localhost:9080/ POST 1 Sedia Ikea" << endl;
            vector<string> v;
            char in[200];
            cout << " > ";
            cin.getline(in, sizeof(in));
            int check = split(in, &v);
            if(check == 1) break; 
            else if(check == 2) continue;
            string address = v[0];
            string method = v[1];
            vector<Async::Promise<Http::Response>> responses;
            int id = atoi(v[2].c_str());
            if(method == "POST") {
                client.post(address, id, v[3], v[4]);
            } else if (method == "GET"){
                client.get(address, id);
            } else if (method == "DEL"){
                client.del(address, id);
            } else {
                cout << "Metodo errato!" << endl;
                continue;
            }
            auto sync = Async::whenAll(responses.begin(), responses.end());
            Async::Barrier<vector<Http::Response>> barrier(sync);
            barrier.wait_for(chrono::seconds(1));
        } __catch(exception e) {
            cout << "Error: " << e.what() <<endl;
        }
    }
    client.shutdown();
}

/**
 * Funzione main che prende il server ed il client e li esegue parallelamente su due thread differenti.
 */
int main(int argc, char *argv[]) 
{
    bool httpsOn = false;
    if(argc > 1 && string(argv[1]) == "on")
        httpsOn = true;
    thread server(startServer, httpsOn), client(startClient);
    server.join();
    client.join();
}
