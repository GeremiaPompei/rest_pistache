#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

class Element {
    public:
        Element(string* body)
        {
            Document doc;
            doc.Parse(body->c_str());
            id_ = doc["id"].GetInt();
            name_ = doc["name"].GetString();
            description_ = doc["description"].GetString();
        }

        int id() const {
            return id_;
        }

        const std::string& name() const {
            return name_;
        }

        const std::string& description() const {
            return description_;
        }
        
        const std::string stringify() const {
            Document doc;
            doc.SetObject();
            doc.AddMember("id", id_, doc.GetAllocator());
            Value namev;
            namev.SetString(name_.c_str(), name_.length()*sizeof(char),doc.GetAllocator());
            doc.AddMember("name", namev, doc.GetAllocator());
            Value descv;
            descv.SetString(description_.c_str(), description_.length()*sizeof(char),doc.GetAllocator());
            doc.AddMember("description", descv, doc.GetAllocator());
            StringBuffer buffer;
            buffer.Clear();
            Writer<StringBuffer> writer(buffer);
            doc.Accept(writer);
            return buffer.GetString();
        }
    private:
        int id_;
        std::string name_;
        std::string description_;
};
