#ifndef CLIENT_H
#define CLIENT_H

#include <pistache/client.h>
#include "element.h"

using namespace std;
using namespace Pistache;
using namespace rapidjson;

/**
 * Client utile per scambiare messaggi con il server.
 */
class Client {
private:
    
    /**
     * Variabile utile per eseguire le chiamate rest.
     */
    Http::Client *client;
public:
    
    /**
     * Metodo costruttore.
     */
    Client();
    
    /**
     * Metodo utle ad eseguire richieste POST.
     */
    void post(string address, int id, string name, string description);
    
    
    /**
     * Metodo utle ad eseguire richieste GET.
     */
    void get(string address, int id);
    
    
    /**
     * Metodo utle ad eseguire richieste DELETE.
     */
    void del(string address, int id);
    
    /**
     * Metodo tramite il quale viene spento il client.
     */
    void shutdown();
};

#endif
