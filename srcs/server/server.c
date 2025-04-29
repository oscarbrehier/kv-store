#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define PORT 8080

void    *handle_client(void *arg)
{
    int 	client_socket;
	char 	buffer[1024];
	size_t	valread;

	client_socket = *(int *)arg;
	free(arg);
	printf("Client connected (threaded).\n");
	while (1)
	{
		memset(buffer, 0, sizeof(buffer));
        valread = read(client_socket, buffer, sizeof(buffer) - 1);
		if (valread <= 0)
		{
			printf("Client disconnected");
			break ;
		}
		buffer[valread] = '\0';
        buffer[strcspn(buffer, "\r\n")] = '\0';
		printf("Received %s\n", buffer);
		if (strcmp(buffer, "hello") == 0)
        {
            send(client_socket, "Hi there!\n", strlen("Hi there!\n"), 0);
        }
        else if (strcmp(buffer, "time") == 0)
        {
            time_t now = time(NULL);
            char timebuf[128];
            snprintf(timebuf, sizeof(timebuf), "Current time: %s", ctime(&now));
            send(client_socket, timebuf, strlen(timebuf), 0);
        }
        else if (strcmp(buffer, "quit") == 0)
        {
            send(client_socket, "Goodbye!\n", strlen("Goodbye!\n"), 0);
            break;
        }
        else
        {
            send(client_socket, "Unknown command\n", strlen("Unknown command\n"), 0);
        }
	}
	close(client_socket);
	pthread_exit(NULL);
}

int server(void)
{
    int server_fd;
    int *new_socket;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket failed");
        return (1);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("setsockopt SO_REUSEADDR");
        return (1);
    }
    
    #ifdef SO_REUSEPORT
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0)
    {
        perror("setsockopt SO_REUSEPORT");
        return (1);
    }
    #endif
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        return (1);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        return (1);
    }

    printf("Server listening on port %d\n", PORT);

    while (1)
    {
        new_socket = malloc(sizeof(int));
        *new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
        if (*new_socket < 0)
        {
            perror("accept");
            close(*new_socket);
            continue ;
        }

        pthread_t tid;
        if (pthread_create(&tid, NULL, handle_client, new_socket) != 0)
        {
            perror("pthread_create");
            close(*new_socket);
            free(new_socket);
        }
        pthread_detach(tid);
    }

    // closing the connected socket
    // close(new_socket);

    // closing the listening socket
    close(server_fd);
    return 0;
}