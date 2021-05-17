#include "server.h"

/**
 * Metiodo utile ad inizializzare le rotte del server.
 */
void Server::setupRoutes() 
{
    using namespace Rest;
    Routes::Post(router, "/post", Routes::bind(&Server::post, this));
    Routes::Get(router, "/get/:id", Routes::bind(&Server::get, this));
    Routes::Delete(router, "/delete/:id", Routes::bind(&Server::del, this));
}

/**
 * Metodo utile a configurare la risposta ad una richiesta POST.
 */
void Server::post(const Rest::Request& request, Http::ResponseWriter response) 
{
    string body = request.body();
    Element element(body);
    auto it = find_if(elements.begin(), elements.end(), [&](const Element& el) {
        return element.id() == el.id();
    });
    if (it == end(elements)) {
        elements.push_back(element);
        string stringified = element.stringify();
        cout << "ServerPost >> record element: " << stringified << "\n";
        response.send(Http::Code::Created, "Ok!");
    }
    else {
        response.send(Http::Code::Ok, "There is an element with the same id!");
    }
    response.send(Http::Code::Created, element.stringify());
}


/**
 * Metodo utile a configurare la risposta ad una richiesta GET.
 */
void Server::get(const Rest::Request& request, Http::ResponseWriter response) 
{
    int id = request.param(":id").as<int>();
    auto it = find_if(elements.begin(), elements.end(), [&](const Element& el) {
        return el.id() == id;
    });
    if (it == end(elements)) {
        response.send(Http::Code::Not_Found, "Element does not exist");
    } else {
        const Element& element = *it;
        string stringified = element.stringify();
        cout << "ServerGet >> get element: " << stringified << "\n";
        response.send(Http::Code::Ok, stringified);
    }
}


/**
 * Metodo utile a configurare la risposta ad una richiesta DELETE.
 */
void Server::del(const Rest::Request& request, Http::ResponseWriter response) {
    int id = request.param(":id").as<int>();
    auto it = find_if(elements.begin(), elements.end(), [&](const Element& el) {
        return el.id() == id;
    });
    if (it == end(elements)) {
        response.send(Http::Code::Not_Found, "Element does not exist");
    } else {
        const Element& element = *it;
        for (int i = 0; i < elements.size(); ++i) {
            if((elements.begin()+i)->id()==id)
                elements.erase((elements.begin()+i));
        }
        string stringified = element.stringify();
        cout << "ServerDel >> deleted element: " << stringified << "\n";
        response.send(Http::Code::Ok, stringified);
    }
}

/**
 * Costruttore del server.
 */
Server::Server(int nPort)
{ 
    Port port(nPort);
    Address addr(Ipv4::any(), port);
    httpEndpoint = shared_ptr<Http::Endpoint>(make_shared<Http::Endpoint>(addr));
}

/**
 * Metodo utile ad inizializzare il server.
 */
void Server::init(size_t thr) 
{
    auto opts = Http::Endpoint::options().threads(static_cast<int>(thr));
    httpEndpoint->init(opts);
    setupRoutes();
}

/**
 * Metodo utile a far partire il server.
 */
void Server::start(bool httpsOn) 
{
    httpEndpoint->setHandler(router.handler());
    if(httpsOn) {
        httpEndpoint->useSSL("../server/server-crt.pem", "../server/server-key.pem");
        httpEndpoint->useSSLAuth("../rootCA/rootCA-crt.pem");
    }
    httpEndpoint->serve();
}
    
/**
 * Metodo utile a spegnere il server.
 */
void Server::stop() 
{
    httpEndpoint->shutdown();
}
