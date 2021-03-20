#include "utils.h"
#include "element.cpp"

class Server {
private:
    std::vector<Element> elements;
    std::shared_ptr<Http::Endpoint> httpEndpoint;
    Rest::Router router;
    
    void setupRoutes() {
        using namespace Rest;
        Routes::Post(router, "/record", Routes::bind(&Server::doRecordElement, this));
        Routes::Get(router, "/value/:id", Routes::bind(&Server::doGetElement, this));
        Routes::Delete(router, "/del/:id", Routes::bind(&Server::doDeleteElement, this));
    }

    void doRecordElement(const Rest::Request& request, Http::ResponseWriter response) {
        string body = request.body();
        Element element = Element(&body);
        auto it = std::find_if(elements.begin(), elements.end(), [&](const Element& el) {
            return element.id() == el.id();
        });
        if (it == std::end(elements)) {
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

    void doDeleteElement(const Rest::Request& request, Http::ResponseWriter response) {
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

    void doGetElement(const Rest::Request& request, Http::ResponseWriter response) {
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
public:
    explicit Server(Address addr)
        : httpEndpoint(std::make_shared<Http::Endpoint>(addr))
    { }

    void init(size_t thr = 2) {
        auto opts = Http::Endpoint::options()
            .threads(static_cast<int>(thr));
        httpEndpoint->init(opts);
        setupRoutes();
    }

    void start() {
        httpEndpoint->setHandler(router.handler());
        httpEndpoint->serve();
    }
    
    void stop() {
        httpEndpoint->shutdown();
    }
};
