#include "utils.h"
#include "element.hpp"

/**
 * Classe Server utile per rispondere al client.
 */
class Server {
private:
    
    /**
     * Element salvati nel server da parte del client.
     */
    std::vector<Element> elements;
    
    /**
     * Endpoint dell'istanza del server utile per operare sulle funzioni principali del servizio.
     */
    std::shared_ptr<Http::Endpoint> httpEndpoint;
    
    /**
     * Rotte rest messe a disposizione dal server.
     */
    Rest::Router router;
    
    /**
     * Metiodo utile ad inizializzare le rotte del server.
     */
    void setupRoutes() {
        using namespace Rest;
        Routes::Post(router, "/post", Routes::bind(&Server::post, this));
        Routes::Get(router, "/get/:id", Routes::bind(&Server::get, this));
        Routes::Delete(router, "/delete/:id", Routes::bind(&Server::del, this));
    }

    /**
     * Metodo utile a configurare la risposta ad una richiesta POST.
     */
    void post(const Rest::Request& request, Http::ResponseWriter response) {
        string body = request.body();
        Element *element = new Element(&body);
        auto it = std::find_if(elements.begin(), elements.end(), [&](const Element& el) {
            return element->id() == el.id();
        });
        if (it == std::end(elements)) {
            elements.push_back(*element);
            string stringified = element->stringify();
            cout << "ServerPost >> record element: " << stringified << "\n";
            response.send(Http::Code::Created, "Ok!");
        }
        else {
            response.send(Http::Code::Ok, "There is an element with the same id!");
        }
        response.send(Http::Code::Created, element->stringify());
    }


    /**
     * Metodo utile a configurare la risposta ad una richiesta GET.
     */
    void get(const Rest::Request& request, Http::ResponseWriter response) {
        int id = request.param(":id").as<int>();
        auto it = std::find_if(elements.begin(), elements.end(), [&](const Element& el) {
            return el.id() == id;
        });
        if (it == std::end(elements)) {
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
    void del(const Rest::Request& request, Http::ResponseWriter response) {
        int id = request.param(":id").as<int>();
        auto it = std::find_if(elements.begin(), elements.end(), [&](const Element& el) {
            return el.id() == id;
        });
        if (it == std::end(elements)) {
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
public:
    /**
     * Costruttore del server.
     */
    explicit Server(Address addr)
        : httpEndpoint(std::make_shared<Http::Endpoint>(addr))
    { }

    /**
     * Metodo utile ad inizializzare il server.
     */
    void init(size_t thr = 2) {
        auto opts = Http::Endpoint::options()
            .threads(static_cast<int>(thr));
        httpEndpoint->init(opts);
        setupRoutes();
    }

    /**
     * Metodo utile a far partire il server.
     */
    void start() {
        httpEndpoint->setHandler(router.handler());
        httpEndpoint->serve();
    }
    
    /**
     * Metodo utile a spegnere il server.
     */
    void stop() {
        httpEndpoint->shutdown();
    }
};
