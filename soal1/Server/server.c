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
#include <fcntl.h>

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

void read_tsv_line(Entry *store, char *line) {
    int i = 0;
    while (line[i] != '\t') {
        store->publisher[i] = line[i];
        i++;
    }
    store->publisher[i] = '\0';
    i++;
    int j = 0;

    while (line[i] != '\t') {
        store->year[j] = line[i];
        i++;
        j++;
    }

    store->year[j] = '\0';
    i++;
    j = 0;

    while (line[i] != '\n') {
        store->path[j] = line[i];
        i++;
        j++;
    }

    store->path[j] = '\0';

    return;
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

                        char dest[STR_SIZE] = "FILES/";
                        strcat(dest, request.path);

                        int des_fd = open(dest, O_WRONLY | O_CREAT | O_EXCL, 0700);
                        if (!des_fd) {
                            perror("can't open file");
                            exit(EXIT_FAILURE);
                        }

                        int file_read_len;
                        char buff[STR_SIZE];
                        while (true) {
                            memset(buff, 0x00, STR_SIZE);
                            file_read_len = read(new_socket, buff, STR_SIZE);
                            write(des_fd, buff, file_read_len);

                            //temporary fix
                            break;
                            if (file_read_len == 0) {
                                printf("finish\n");
                                break;
                            }
                        }

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
                            Entry temp_entry;
                            read_tsv_line(&temp_entry, line);

                            char ext[SIZE];
                            int i = 0;
                            while (temp_entry.path[i] != '.') {
                                i++;
                            }
                            int j = 0;
                            while (temp_entry.path[i] != '\0') {
                                ext[j] = temp_entry.path[i];
                                i++;
                                j++;
                            }

                            ext[j] = '\0';

                            char message[STR_SIZE];
                            sprintf(message, "Nama : %s\nPublisher : %s\nTahun Publishing : %s\nEkstensi File : %s\nFilepath : %s\n\n", 
                                    temp_entry.path, temp_entry.publisher, temp_entry.year, ext, temp_entry.path);
                            
                            // printf("%s", message);
                            send(new_socket, message, STR_SIZE, 0);
                        }
                        send(new_socket, "e", sizeof("e"), 0);
                        fclose(fp);
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
