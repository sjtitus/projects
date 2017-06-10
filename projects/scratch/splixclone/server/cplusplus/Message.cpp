//_____________________________________________________________________________
// Message 
// Message class for IPC. 
//_____________________________________________________________________________
#include <cstdio>
#include <iostream>
#include <sstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/map.hpp>

#include "Message.hpp"
#include "Exception.hpp"

//_________________________________________________________________________
// [] operator: Access message elements by key. 
// Note: referring to a non-existent key creates an entry for the key 
// with an empty string value.
std::string& Message::operator[] (const std::string &key)
{
    return keyValueMap_[key];
}

//_________________________________________________________________________
// Keys: Returns a vector of message keys 
// creates an entry for the key with an empty string value.
const std::vector<std::string> Message::Keys()
{
    try
    {
        std::vector<std::string> v;
        for(std::map<std::string,std::string>::iterator it = keyValueMap_.begin(); it != keyValueMap_.end(); ++it) {
            v.push_back(it->first);
        }
        return v;
    }
    catch (std::exception &e)
    {
        RETHROW_SYSTEM_EXCEPTION(e);
    }
}

//_________________________________________________________________________
// Map: accessor for the underlying std::map 
std::map<std::string, std::string>& Message::Map()
{
    return keyValueMap_;
}

//______________________________________________________________________________
// Deserialize: construct a message from a serialized string.
// Returns a smart pointer the new Message.
Message::MessagePtr Message::Deserialize(const std::string &s)
{
    try
    {
        std::stringstream ss(s);
        Message::MessagePtr p(new Message());
        {
            boost::archive::text_iarchive ia(ss);
            ia >> *p;
        } 
        return p;
    }
    catch (std::exception &e)
    {
        RETHROW_SYSTEM_EXCEPTION(e);
    }
}

//______________________________________________________________________________
// Serialize: Serialize a message into a string. 
void Message::Serialize(std::string &header, std::string &payload)
{
    try
    {
        // create payload
        std::stringstream ss;
        {
            boost::archive::text_oarchive oa(ss);
            oa << (*this);
        }
        payload = ss.str();
       
        // create header 
        std::ostringstream header_stream;
        header_stream << std::setw(Message::HEADER_LENGTH) << std::hex << payload.size();
        header = header_stream.str();
    }
    catch (std::exception &e)
    {
        RETHROW_SYSTEM_EXCEPTION(e);
    }
}

    
//______________________________________________________________________________
// serialize: private boost serialization support 
template<class Archive> void Message::serialize(Archive & ar, const unsigned int version)
{
    ar & keyValueMap_; 
}
    
//______________________________________________________________________________
// Print: print Message to cout 
void Message::Print()
{
    std::cout << "Message:\n";
    for(Message::it_type iterator = keyValueMap_.begin(); iterator != keyValueMap_.end(); iterator++) 
    {
        std::cout << "'" << iterator->first << "' = " << "'" << iterator->second << "'\n";
    }
}
        

    
#ifdef _UNIT_TEST_MESSAGE_

int main(int argc, const char **argv)
{

    try
    {
        // Create a message with some keys and values
        Message msg;
        std::string nestr = msg["key9"];
        msg["key1"] = "this is key1 value";   
        msg["key2"] = "this is key2 value";   
        msg["key3"] = "this is key3 value";   
       
        std::cout << "== Starting Message ==\n";
        msg.Print(); 

        std::string msg_header_string;
        std::string msg_payload_string;
        msg.Serialize(msg_header_string,msg_payload_string);
        std::cout << "Serialized header:'" << msg_header_string << "'\n";
        std::cout << "Serialized payload:'" << msg_payload_string << "'\n";

        Message::MessagePtr pMsg = Message::Deserialize(msg_payload_string);
        std::cout << "== Deserialized Message ==\n";
        pMsg->Print();
 
        return 0;
    }
    catch (SystemException &e)
    {
        std::cerr << e.Info();
    }
}

#endif
