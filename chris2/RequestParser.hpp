#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>

#include <vector>

class RequestParser {

    public:

        RequestParser( std::string buffer, int socket );
        void tokenizeRequest( void );

    private:

        std::string _buffer;
        int _socket;

        template <typename Key, typename Value>
        struct Node {
            Key key;
            Value value;
            Node<Key, Value>* next;

            Node( Key key, Value value, Node<Key, Value>* next = nullptr )
                : key(key), value(value), next(next) {}
        };


};