#include "server/sync_drive_server.h"

int main(int argc, char** argv){
    int port = atoi(argv[1]);
    std::cout << port << std::endl;
    SyncDriveServer server(port);
    server.Start();
    return 0;
}
