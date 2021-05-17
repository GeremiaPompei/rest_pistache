#include "client.h"

/**
 * Metodo costruttore.
 */
Client::Client()
{
    client = new Http::Client();
    auto opts = Http::Client::options()
        .threads(1)
        .maxConnectionsPerHost(8);
    client->init(opts);
}

    
/**
 * Metodo utle ad eseguire richieste POST.
 */
void Client::post(string address, int id, string name, string description) 
{
    Element el(id, name, description);
    string json = el.stringify();
    auto header = Http::Header::ContentType();
    auto mime = Http::Mime::MediaType::fromString("application/json");
    header.setMime(mime);
    std::cout<<json<<std::endl;
    auto resp = client->post(address+"post")
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
    
/**
 * Metodo utle ad eseguire richieste GET.
 */
void Client::get(string address, int id) 
{
    auto resp = client->get(address+"get/"+to_string(id)).send();
    resp.then(
        [&](Http::Response response) {
            std::cout<<"Response: "<<response.code()<<"\nBody: "<<response.body()<<"\n";
        },
        [&](std::exception_ptr exc) {
            PrintException excPrinter;
            excPrinter(exc);
        });
}
    
    
/**
 * Metodo utle ad eseguire richieste DELETE.
 */
void Client::del(string address, int id) 
{
    auto resp = client->del(address+"delete/"+to_string(id)).send();
    resp.then(
        [&](Http::Response response) {
            std::cout<<"Response: "<<response.code()<<"\nBody: "<<response.body()<<"\n";
        },
        [&](std::exception_ptr exc) {
            PrintException excPrinter;
            excPrinter(exc);
        });
}


/**
 * Metodo tramite il quale viene spento il client.
 */
void Client::shutdown()
{
    client->shutdown();
}
