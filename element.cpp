#include "element.h"

Element::Element(int id, string name, string description) {
    id_ = id,
    name_ = name;
    description_ = description;
}
        
Element::Element(string body) {
    Document doc;
    doc.Parse(body.c_str());
    id_ = doc["id"].GetInt();
    name_ = doc["name"].GetString();
    description_ = doc["description"].GetString();
}

int Element::id() const {
    return id_;
}

const string& Element::name() const {
    return name_;
}

const string& Element::description() const {
    return description_;
}
        
const string Element::stringify() const {
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
