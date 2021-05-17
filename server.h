#ifndef SERVER_H
#define SERVER_H

#include <pistache/router.h>
#include <pistache/endpoint.h>

#include "element.h"

using namespace std;
using namespace Pistache;

/**
 * Classe Server utile per rispondere al client.
 */
class Server {
private:
    
    /**
     * Element salvati nel server da parte del client.
     */
    vector<Element> elements;
    
    /**
     * Endpoint dell'istanza del server utile per operare sulle funzioni principali del servizio.
     */
    shared_ptr<Http::Endpoint> httpEndpoint;
    
    /**
     * Rotte rest messe a disposizione dal server.
     */
    Rest::Router router;
    
    /**
     * Metiodo utile ad inizializzare le rotte del server.
     */
    void setupRoutes();

    /**
     * Metodo utile a configurare la risposta ad una richiesta POST.
     */
    void post(const Rest::Request& request, Http::ResponseWriter response);


    /**
     * Metodo utile a configurare la risposta ad una richiesta GET.
     */
    void get(const Rest::Request& request, Http::ResponseWriter response);


    /**
     * Metodo utile a configurare la risposta ad una richiesta DELETE.
     */
    void del(const Rest::Request& request, Http::ResponseWriter response);
public:
    /**
     * Costruttore del server.
     */
    Server(int nPort);

    /**
     * Metodo utile ad inizializzare il server.
     */
    void init(size_t thr = 2);

    /**
     * Metodo utile a far partire il server.
     */
    void start(bool httpsOn);
    
    /**
     * Metodo utile a spegnere il server.
     */
    void stop();
};

#endif
