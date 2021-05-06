#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>
#include <stdbool.h>

#define SIZE 50
#define STR_SIZE 1024
#define PORT 8080
#define N_OF_CONNECTION 5

pthread_t tid[3000];
int find;

typedef struct akun_t {
    char name[SIZE];
    char password[SIZE];
} Account;

typedef struct file_t
{
    char publisher[SIZE];
    char year[SIZE];
    char path[SIZE];
} Entry;


bool equal(char *s1, char *s2) {
    int i = 0, \
        d = 0;
    
    for (i = 0; s1[i] != '\0'; i++) {
        if (toupper(s1[i]) != toupper(s2[i]))
            return false;
    }
    return true;
}

void *client(void *tmp) {
    FILE *fp, \
        *fp2, \
        *fp3;
    
    Account akun, akun2;

    char buffer[STR_SIZE] = {0};
    char hello[STR_SIZE];

    char *regist = "register";
    char *login = "login";

    int valread;
    int new_socket = *(int *)tmp;

    while (true) {
        valread = read(new_socket, buffer, STR_SIZE);

        if (equal(login, buffer)) {
            valread = read(new_socket, akun.name, STR_SIZE);
            valread = read(new_socket, akun.password, STR_SIZE);

            fp3 = fopen("akun.txt", "r");

            int flag = 0;
            char *line = NULL;
            ssize_t len = 0;
            ssize_t file_read;
            while ((file_read = getline(&line, &len, fp3) != -1)) {
                char t_name[SIZE];
                char t_pass[SIZE];

                int i = 0;
                while (line[i] != ':') {
                    t_name[i] = line[i];
                    i++;
                }
                t_name[i] = '\0';
                i++;
                int j = 0;
                while (line[i] != '\n') {
                    t_pass[j] = line[i];
                    j++;
                    i++;
                }
                t_pass[j] = '\0';

                if (equal(akun.name, t_name) && equal(akun.password, t_pass)) {
                    flag = 1;
                    char *buffer = "Login success";
                    send(new_socket, buffer, strlen(buffer), 0);
                    break;
                }
            }

            if (flag == 0) {
                printf("Auth Failed\n");

                char *buffer = "Login failed";
                send(new_socket, buffer, strlen(buffer), 0);
            }
            else {
                printf("Auth success\n");

                while (true) {
                    valread = read(new_socket, buffer, STR_SIZE);
                    if (equal("logout", buffer)) {
                        break;
                    }
                    else if (equal("add", buffer)) {
                        Entry request;

                        valread = read(new_socket, request.publisher, STR_SIZE);
                        valread = read(new_socket, request.year, STR_SIZE);
                        valread = read(new_socket, request.path, STR_SIZE);

                        fp = fopen("files.tsv", "a");
                        fprintf(fp, "%s\t%s\t%s\n", request.publisher, request.year, request.path);
                        fclose(fp);
                        continue;
                    }
                    else if (equal("see", buffer)) {
                        fp = fopen("files.tsv", "r");

                        char *line = NULL;
                        ssize_t len = 0;
                        ssize_t file_read;
                        while ((file_read = getline(&line, &len, fp) != -1)) {
                            printf("line: %s", line);
                        }
                        char *buffer = "see";

                        fclose(fp);

                        send(new_socket, buffer, strlen(buffer), 0);
                    }
                }
            }
            fclose(fp3);
        }
        if (equal(regist, buffer)) {
            fp2 = fopen("akun.txt", "a");

            valread = read(new_socket, akun.name, STR_SIZE);
            valread = read(new_socket, akun.password, STR_SIZE);

            fprintf(fp2, "%s:%s\n", akun.name, akun.password);
            fclose(fp2);
        }
    }
}

int main(int argc, char const *argv[]) {
    int server_fd, \
        new_socket, \
        valread;
    int opt = 1;

    struct sockaddr_in address;
    int addr_len = sizeof(address);

    find = 0;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(server_fd, SOL_SOCKET, 
        SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0 ) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, N_OF_CONNECTION) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    mkdir("FILES", 0777);

    int total = 0;
    while(true) {
        if ((new_socket = accept(server_fd, 
            (struct sockaddr *) &address, (socklen_t*) &addr_len)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        pthread_create(&(tid[total]), NULL, &client, &new_socket);
        total++;
    }

    return 0;
}
