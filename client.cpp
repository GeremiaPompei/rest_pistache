#include "utils.h"

class Client {
private:
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

    int split(string str, vector<string> *v) {
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
        return 0;
    }
    
    void post(Http::Client *client, string address, int id, string name, string description) {
        std::string json = toJson(id, name.c_str(), description.c_str());
        auto header = Http::Header::ContentType();
        auto mime = Http::Mime::MediaType::fromString("application/json");
        header.setMime(mime);
        std::cout<<json<<std::endl;
        auto resp = client->post(address+"record")
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
    
    void get(Http::Client *client, string address, int id) {
        auto resp = client->get(address+"value/"+to_string(id)).send();
        resp.then(
            [&](Http::Response response) {
                std::cout<<"Response: "<<response.code()<<"\nBody: "<<response.body()<<"\n";
            },
            [&](std::exception_ptr exc) {
                PrintException excPrinter;
                excPrinter(exc);
            });
    }
    
    void del(Http::Client *client, string address, int id) {
        auto resp = client->del(address+"del/"+to_string(id)).send();
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
    void start() {
        Http::Client client;
        auto opts = Http::Client::options()
            .threads(1)
            .maxConnectionsPerHost(8);
        client.init(opts);
        while(1==1) {
            try {
                cout << "Inserisci dominio di destinazione(http://ip:port/), metodo(POST, GET, DEL), id dell'elemento e in caso di POST nome e descrizione. exit per uscire." << endl;
                cout << "Esempio: http://localhost:9080/ POST 1 Sedia Ikea" << endl;
                vector<string> v;
                char in[200];
                cout << " > ";
                cin.getline(in, sizeof(in));
                if(split(in, &v) != 0) break;
                std::string address = v[0];
                std::string method = v[1];
                std::vector<Async::Promise<Http::Response>> responses;
                int id = atoi(v[2].c_str());
                if(method == "POST") {
                    post(&client, address, id, v[3], v[4]);
                } else if (method == "GET"){
                    get(&client, address, id);
                } else if (method == "DEL"){
                    del(&client, address, id);
                } else {
                    cout << "Metodo errato!" << endl;
                    continue;
                }
                auto sync = Async::whenAll(responses.begin(), responses.end());
                Async::Barrier<std::vector<Http::Response>> barrier(sync);
                barrier.wait_for(std::chrono::seconds(1));
            } __catch(exception e) {
                cout << "Error: " << e.what() <<endl;
            }
        }
        client.shutdown();
    }
};
