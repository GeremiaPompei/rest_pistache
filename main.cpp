#include "utils.h"
#include "element.cpp"

class StatsEndpoint {
public:
    explicit StatsEndpoint(Address addr)
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

private:
    void setupRoutes() {
        using namespace Rest;

        Routes::Post(router, "/record", Routes::bind(&StatsEndpoint::doRecordElement, this));
        Routes::Get(router, "/value/:id", Routes::bind(&StatsEndpoint::doGetElement, this));
        Routes::Delete(router, "/del/:id", Routes::bind(&StatsEndpoint::doDeleteElement, this));

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
    
    std::vector<Element> elements;
    std::shared_ptr<Http::Endpoint> httpEndpoint;
    Rest::Router router;
};

int main(int argc, char *argv[]) {
    int nport = 9080;
    Port port(nport);
    int thr = 2;
    int flag = 0;
    while(flag==0) {
        try {
            if (argc >= 2) {
                port = static_cast<uint16_t>(std::stol(argv[1]));
                if (argc == 3)
                    thr = std::stoi(argv[2]);
            }
            Address addr(Ipv4::any(), port);
            StatsEndpoint stats(addr);
            flag=1;
            cout << "Cores = " << hardware_concurrency() << endl;
            cout << "Using " << thr << " threads" << endl;
            cout << "Server started on port: " << port << endl;
            stats.init(thr);
            stats.start();
        } __catch(exception e) {
            port = Port(++nport);
        }
    }
}
