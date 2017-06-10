#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <string>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/serialization/map.hpp>
 
//______________________________________________________________________________
// Message
// A message is an std::map(string,string) serialized using boost. 
class Message
{
  public:

    static const std::size_t HEADER_LENGTH = 8;
    typedef boost::shared_ptr<Message> MessagePtr; 
    typedef std::map<std::string, std::string>::iterator it_type;

    //_________________________________________________________________________
    // [] operator: Access message elements by key. 
    // Note: referring to a non-existent key creates an entry for the key 
    // with an empty string value.
    std::string& operator[] (const std::string &key);

    //_________________________________________________________________________
    // Keys: Returns a vector of message keys 
    // creates an entry for the key with an empty string value.
    const std::vector<std::string> Keys();

    //_________________________________________________________________________
    // Map: accessor for the underlying std::map holding keys/values 
    std::map<std::string, std::string>& Map();

    //______________________________________________________________________________
    // Deserialize: construct a message from a serialized string.
    static Message::MessagePtr Deserialize(const std::string &s);

    //______________________________________________________________________________
    // Serialize: Serialize a message into 2 strings: a header string of  
    // length HEADER_LENGTH that contains the length of the payload string, and 
    // a payload string containing the serialized Message.
    void Serialize(std::string &header, std::string &payload);
    
    //______________________________________________________________________________
    // Print: print Message to cout 
    void Print();


  private:
    std::map<std::string, std::string> keyValueMap_; 
   
    // Serialization
    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive & ar, const unsigned int version);
};




#endif
