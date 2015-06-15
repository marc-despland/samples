#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

int main(int argc, char **argv)
{
    int res;
    struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
    char buf[1024] = { 0 };
    int s, client, bytes_read;
    socklen_t opt = sizeof(rem_addr);
    char dest[18] = "00:1A:7D:DA:71:14";

    // allocate socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // bind socket to port 1 of the first available 
    // local bluetooth adapter
    loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_bdaddr = *BDADDR_ANY;
    //str2ba( dest, &loc_addr.rc_bdaddr );
    loc_addr.rc_channel = (uint8_t) 2;
    
    res=bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));
    printf("Bind : %d\n",res);
    // put socket into listening mode
    res= listen(s, 2);
    printf("Listen : %d\n",res);
    // accept one connection
    client = accept(s, (struct sockaddr *)&rem_addr, &opt);

    printf("accept\n");
    ba2str( &rem_addr.rc_bdaddr, buf );
    fprintf(stderr, "accepted connection from %s\n", buf);
    memset(buf, 0, sizeof(buf));

    // read data from the client
    bytes_read = read(client, buf, sizeof(buf));
    if( bytes_read > 0 ) {
        printf("received [%s]\n", buf);
    }

    // close connection
    close(client);
    close(s);
    return 0;
}
