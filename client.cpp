#include "utils.h"

class Client {
private:
    Http::Client client;
    
    std::string toJson(int id, std::string name, std::string description) {
        Document doc;
        doc.SetObject();
        doc.AddMember("id", id, doc.GetAllocator());
        Value namev;
        namev.SetString(name.c_str(), name.length()*sizeof(char), doc.GetAllocator());
        doc.AddMember("name", namev, doc.GetAllocator());
        Value descv;
        descv.SetString(description.c_str(), description.length()*sizeof(char), doc.GetAllocator());
        doc.AddMember("description", descv, doc.GetAllocator());
        StringBuffer buffer;
        buffer.Clear();
        Writer<StringBuffer> writer(buffer);
        doc.Accept(writer);
        return buffer.GetString();
    }

    void split(string str, vector<string> v) {
        string token = "";
        for(auto c : str) { 
            if(c == ' ') {
                v.push_back(token);
            } else {
                token += c;
            }
        } 
    }
    
    void post(string address, int id, string name, string description) {
        std::string json = toJson(id, name.c_str(), description.c_str());
        auto header = Http::Header::ContentType();
        auto mime = Http::Mime::MediaType::fromString("application/json");
        header.setMime(mime);
        std::cout<<json<<std::endl;
        auto resp = client.post(address+"record")
            .header<Http::Header::ContentType>(header)
            .body(json)
            .send();
        resp.then(
            [&](Http::Response response) {
                std::cout<<"Response: "<<response.code()<<"\nBody: "<<response.body()<<"\n";
            },
            [&](std::exception_ptr exc) {
                PrintException excPrinter;
                excPrinter(exc);
            });
    }
    
    void get(string address, int id) {
        auto resp = client.get(address+"value/"+to_string(id)).send();
        resp.then(
            [&](Http::Response response) {
                std::cout<<"Response: "<<response.code()<<"\nBody: "<<response.body()<<"\n";
            },
            [&](std::exception_ptr exc) {
                PrintException excPrinter;
                excPrinter(exc);
            });
    }
    
    void del(string address, int id) {
        auto resp = client.del(address+"del/"+to_string(id)).send();
        resp.then(
            [&](Http::Response response) {
                std::cout<<"Response: "<<response.code()<<"\nBody: "<<response.body()<<"\n";
            },
            [&](std::exception_ptr exc) {
                PrintException excPrinter;
                excPrinter(exc);
            });
    }

public:
    Client() {
        auto opts = Http::Client::options()
            .threads(1)
            .maxConnectionsPerHost(8);
        client.init(opts);
    }
    
    ~Client() {
        client.shutdown();
    }
    
    void start() {
        while(1) {
            try {
                cout << "Inserisci dominio di destinazione(http://ip:port/), metodo(POST, GET, DEL), id dell'elemento e in caso di POST nome e descrizione. exit per uscire." << endl;
                string input;
                vector<string> v;
                cin >> input;
                if(input == "exit") break;
                split(input, v);
                std::string address = v[0];
                std::string method = v[1];
                std::vector<Async::Promise<Http::Response>> responses;
                int id = atoi(v[2].c_str());
                if(method == "POST") {
                    post(address, id, v[3], v[4]);
                } else if (method == "GET"){
                    get(address, id);
                } else if (method == "DEL"){
                    del(address, id);
                }
                auto sync = Async::whenAll(responses.begin(), responses.end());
                Async::Barrier<std::vector<Http::Response>> barrier(sync);
                barrier.wait_for(std::chrono::seconds(5));
            } __catch(exception e) {
                cout << e.what() <<endl;
            }
        }
    }
};
