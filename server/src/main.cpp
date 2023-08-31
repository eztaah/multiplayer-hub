#include "serve.hpp"


int main() {

    Server server = Server();

    while (true) 
        server.Update();
    
    return 0;
}
