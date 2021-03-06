#ifndef ELEMENT_H
#define ELEMENT_H

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

using namespace std;
using namespace rapidjson;

/**
 * Elemento inviato tra client e server.
 */
class Element {
    
    private:
        
        /**
         * Id dell'Element.
         */
        int id_;
        
        /**
         * Nome dell'Element.
         */
        string name_;
        
        /**
         * Descrizione dell'Element.
         */
        string description_;
        
    public:
        
        /**
         * Costruttore base dell'Element.
         */
        Element(int id, string name, string description);
        
        /**
         * Costruttore utile a inizializzare un'istanza di un Element data una stringa formattata in json.
         */
        Element(string body);
        
        /**
         * Metodo get dell'Id dell'Element.
         */
        int id() const;
        
        /**
         * Metodo get del Name dell'Element.
         */
        const std::string& name() const;
        
        /**
         * Metodo get della Descrizione dell'Element.
         */
        const std::string& description() const;
        
        /**
         * Metodo utile a formattare l'Element come una stringa json.
         */
        const std::string stringify() const;
};

#endif
