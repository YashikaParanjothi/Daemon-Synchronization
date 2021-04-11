#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#define PORT 8080
int main()
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Message from server";

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    if (bind(server_fd, (struct sockaddr *)&address,sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    valread = read( new_socket , buffer, 1024);
        FILE *fp= NULL;
        pid_t process_id = 0;
        pid_t session_id = 0;

        process_id = fork();

        if (process_id < 0)
        {
                printf("fork failed!\n");
                exit(1);
        }

        if (process_id > 0)
        {
                exit(0);
        }

        //set new session
        session_id = setsid();
        if(session_id < 0)
        {
                exit(1);
        }

        close(0);
        close(1);
        close(2);

        fp = fopen ("LogFile.txt", "a");

        time_t curr_time;
        time(&curr_time);
        fprintf(fp,"%s---%s---%s\n",ctime(&curr_time),buffer,"Client in execution");

        sleep(3);
        send(new_socket , hello , strlen(hello) , 0 );
        time(&curr_time);
        fprintf(fp,"%s---%s---%s\n",ctime(&curr_time),buffer,"Client Completed");
        fclose(fp);
        return 0;
}

