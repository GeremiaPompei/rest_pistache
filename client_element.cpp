#include <atomic>

#include <pistache/client.h>
#include <pistache/http.h>
#include <pistache/net.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

using namespace Pistache;
using namespace Pistache::Http;
using namespace rapidjson;

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

int main(int argc, char *argv[]) {
    
    std::string address = argv[1];
    std::string method = argv[2];
    
    Http::Client client;

    auto opts = Http::Client::options()
        .threads(1)
        .maxConnectionsPerHost(8);
    client.init(opts);

    std::vector<Async::Promise<Http::Response>> responses;
    
    if(method=="POST") {
  
        int id = atoi(argv[3]);
        char* name = argv[4];
        char* description = argv[5];
        
        std::string json = toJson(id, name, description);
        
        auto header = Header::ContentType();
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
    } else if(method=="GET") {
        std::string value = argv[3];
        
        auto resp = client.get(address+"value/"+value).send();
        
        resp.then(
            [&](Http::Response response) {
                std::cout<<"Response: "<<response.code()<<"\nBody: "<<response.body()<<"\n";
            },
            [&](std::exception_ptr exc) {
                PrintException excPrinter;
                excPrinter(exc);
            });
    } else if(method=="DEL") {
        std::string value = argv[3];
        
        auto resp = client.del(address+"del/"+value).send();
        
        resp.then(
            [&](Http::Response response) {
                std::cout<<"Response: "<<response.code()<<"\nBody: "<<response.body()<<"\n";
            },
            [&](std::exception_ptr exc) {
                PrintException excPrinter;
                excPrinter(exc);
            });
    }
    

    auto sync = Async::whenAll(responses.begin(), responses.end());
    Async::Barrier<std::vector<Http::Response>> barrier(sync);
    barrier.wait_for(std::chrono::seconds(5));
    
    client.shutdown();
}
