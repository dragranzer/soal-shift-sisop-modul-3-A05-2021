# soal-shift-sisop-modul-3-A05-2021

## Soal 1
* ### a - Register, Login, >1 Client
  #### Deskripsi Soal
  * Server dapat menerima lebih dari 1 koneksi Client sekaligus, namun hanya 1 Client yang dapat menggunakan fungsi-fungsi program. Client ke-2 dan seterusnya harus menunggu sampai Client pertama log out
  * Client diminta `id` dan `password` saat register -> disimpan di file `akun.txt` sebagai
    <pre>
    id:password
    id2:password2
    </pre>
  * Client diminta `id` dan `password` saat login -> dicocokkan dengan `id:password` yang ada di file `akun.txt`
  
  #### Solusi Soal
  * Konesi Client-Server
    * gunakan template koneksi socket Client dan Server yang ada di modul 3
    * manfaatkan thread untuk dapat memungkinkan terjadinya >1 koneksi Client ke Server
      ```c
      while(true) {
        if ((new_socket = accept(server_fd, 
            (struct sockaddr *) &address, (socklen_t*) &addr_len)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        pthread_create(&(tid[total]), NULL, &client, &new_socket);
        total++;
      }
      ```
    * simpan jumlah Client yang sedang terkoneksi pada suatu variabel
    * tentukan apakah Client akan di terima atau di tahan aksesnya ke fungsi program berdasarkan jumlah koneksi
      ```c
      if (total == 1) {
        send(new_socket, hello, STR_SIZE, 0);
      }
      else {
          send(new_socket, deny, STR_SIZE, 0);
      }
      ```
    * di sisi Client, akan diperiksa pesan yang diterima dari Server
      ```c
      while (!equal(hello, buffer))
      ```
      jika Server tidak mengirim pesan yang menandakan Client dapat menggunakan program, tunggu di `while` loop
    * kembali di sisi Server, selain meng-handle Client pertama, buat Client tersebut untuk menunggu
      ```c
      while (total > 1) {
        valread = read(new_socket, buffer, STR_SIZE);
        if (total == 1) {
            send(new_socket, hello, STR_SIZE, 0);
        }
        else {
            send(new_socket, deny, STR_SIZE, 0);
        }
      }
      ```
      ![image](https://user-images.githubusercontent.com/43901559/118665759-c7771f80-b81c-11eb-878f-450c16789cba.png)
      >tampilan di sisi Client
    * ketika meng-handle Client pertama, siapkan Server untuk menunggu command dari Client. Seperti: login, register, dst.
      
      ![image](https://user-images.githubusercontent.com/43901559/118665671-ae6e6e80-b81c-11eb-84aa-8a34d7bd037a.png)
      >tampilan di sisi Client
    * ketika Client memutus koneksinya, decrement jumlah koneksi
      ```c
      else if (equal(quit, buffer)) {
        close(new_socket);
        total--;
        break;
      }
      ```
    * terima Client yang sedang menunggu apabila menjadi koneksi pertama
      
      ![image](https://user-images.githubusercontent.com/43901559/118665952-f7262780-b81c-11eb-8d51-afc44d12f646.png)
      >tampilan di sisi Client
  * Fungsi register
    * Client mengirimkan command `register` ke Server
    * Server menerima command dan menyiapkan file `akun.txt`
      ```c
      fp2 = fopen("akun.txt", "a");
      ```
    * Client-Server kirim-terima username dan password yang ingin di-regist
      
      ![image](https://user-images.githubusercontent.com/43901559/118666528-761b6000-b81d-11eb-9c2e-ff0565cb28cc.png)
      >tampilan di Client
    * `fprintf`-kan user creds yang baru ke file `akun.txt`
    * `close` file dan akun telah diregister
  * Fungsi login
    * Client mengirimkan command `login` ke Server
    * Server menerima command
    * Client-Server kirim-terima username dan password
      
      ![image](https://user-images.githubusercontent.com/43901559/118667260-0ce81c80-b81e-11eb-9395-485faa5c2610.png)
      >tampilan di Client
    * Server membuka file `akun.txt`
    * Baca file line-by-line dan cari login credentials yang sesuai
      ```c
      while ((file_read = getline(&line, &len, fp3) != -1))
      ```
    * Kirim pesan ke Client apakah login berhasil atau tidak
    * Jika login berhasil, siapkan Client-Server untuk command add, download, dst.
      
      ![image](https://user-images.githubusercontent.com/43901559/118667562-520c4e80-b81e-11eb-95a5-5c7980442145.png)
      >tampilan di Client
  #### Kendala Soal
  * sulit untuk menerapkan fungsionalitas Client yang terkoneksi >1 dan ada Client yang harus menunggu
  
* ### b - files.txt dan folder FILES
  #### Deskripsi Soal
  * di Server terdapat `files.tsv` yang menyimpan
    <pre>
    path file di server
    publisher
    tahunpublikasi
    </pre>
    `files.tsv` di-update setiap ada operasi add dan delete files
  * folder `FILES` yang menyimpan semua file yang dikirimkan oleh Client, otomatis dibuat saat server dijalankan
  
  #### Solusi Soal
  * `files.tsv`
    * pada operasi penambahan file, tambahkan kode yang akan membuat file `files.tsv`
    * perubahan pada file tersebut akan dijelaskan lebih lanjut pada bagian soal selanjutnya
  * folder `FILES`
    * buat folder `FILES` setelah Server dijalankan
  
  #### Kendala Soal
  * tidak ada
  
* ### c - uploading files
  #### Deskripsi Soal
  * file yang dikirim ke Server disimpan di folder `FILES`
    <pre>
    File1.ekstensi
    File2.ekstensi
    </pre>
  * client mengirimkan command `add` dan Client memasukkan detail file
    <pre>
    Publisher:
    Tahun Publikasi:
    Filepath:
    </pre>
  * `files.tsv` menyimpan data file yang dikirimkan ke Server
  
  #### Solusi Soal
  * command `add` dari Client
    * Client menerima command dan meneruskannya ke Server
    * Server bersiap untuk menerima detail dari file
    * Client mengirimkan detail dari file
    
      ![image](https://user-images.githubusercontent.com/43901559/118669933-5df91000-b820-11eb-9ba3-0f5677b741db.png)
      >tampilan di Client
  * pengiriman file ke Server
    * Client membuka file dan mengirimkan file ke Server
      ```c
      int fd = open(data, O_RDONLY);
      if (!fd) {
        perror("can't open");
        exit(EXIT_FAILURE);
      }
      
      int read_len;
      while (true) {
        memset(data, 0x00, STR_SIZE);
        read_len = read(fd, data, STR_SIZE);

        if (read_len == 0) {
          break;
        }
        else {
          send(sock, data, read_len, 0);                               
        }
      }
      close(fd);
      ```
    * Server menerima file dan menyimpannya
      ```c
      int des_fd = open(request.path, O_WRONLY | O_CREAT | O_EXCL, 0700);
      if (!des_fd) {
        perror("can't open file");
        exit(EXIT_FAILURE);
      }
      ```
      >Server bersiap untuk menyimpan file
      
      ![image](https://user-images.githubusercontent.com/43901559/118670586-f1cadc00-b820-11eb-88e5-7b934510e285.png)
      >struktur folder di Server
  * penambahan detail file di `files.tsv`
    * Server menerima detail file terlebih dahulu
      ```c
      valread = read(new_socket, request.publisher, STR_SIZE);
      valread = read(new_socket, request.year, STR_SIZE);
      valread = read(new_socket, clientPath, STR_SIZE);
      ```
    * Server memproses file path dari Client dan memodifikasinya agar dapat menyimpan file di `FILES/nama_file.extension`
    * Server membuka `files.tsv` dan menambahkan detail file
      ```c
      fp = fopen("files.tsv", "a");
      fprintf(fp, "%s\t%s\t%s\n", request.path, request.publisher, request.year);
      fclose(fp);
      ```
      ![image](https://user-images.githubusercontent.com/43901559/118671543-c3013580-b821-11eb-9743-fc1cb1e21e5e.png)
      >isi `files.tsv`
  
  #### Kendala Soal
  * kesulitan mengerjakan soal

  #### Referensi
  * https://stackoverflow.com/questions/40786888/send-file-from-client-to-server-using-socket-in-c-on-linux
  * https://stackoverflow.com/questions/3501338/c-read-file-line-by-line

* ### d - downloading files
  #### Deskripsi Soal
  * Client dapat mendownload file yang ada di Server -> check di `files.tsv`
  
  #### Solusi Soal
  * download file
    * Solusi dari download sangat serupa dengan solusi dari add, hanya saja peran Client-Server dibalik
    * Client mengirimkan command `download nama_file.extension`
    * Server menerima command dan mencari nama file di `files.tsv`
      
      ![image](https://user-images.githubusercontent.com/43901559/118672984-d82a9400-b822-11eb-9a07-3d96c6f27405.png)
      >jika file bisa di-download

      ![image](https://user-images.githubusercontent.com/43901559/118673023-e1b3fc00-b822-11eb-9532-a533eabd6fd6.png)
      >file tidak ada di Server
    * Server akan mengirimkan file ke Client jika ada file yang diminta
  
  #### Kendala Soal
  * kendala serupa dengan kendala di 1.c :)
  
* ### e - deleting files
  #### Deskripsi Soal
  * Client mengirimkan command `delete nama_file.extension`
  * Jika file ada, file hanya di-rename menjadi `old-nama_file.extension`
  * Hapus file dari list pada `files.tsv`
  
  #### Solusi Soal
  * Client send command
    * Client mengirimkan command delete dan nama file yang ingin di-delete
    * Server menerima command dan mengecek apakah ada file tersebut di `files.tsv`
    
      ![image](https://user-images.githubusercontent.com/43901559/118673949-9f3eef00-b823-11eb-97ea-f3af9e775029.png)
      >tampilan di Client apabila file ada dan telah terhapus

      ![image](https://user-images.githubusercontent.com/43901559/118674062-b4b41900-b823-11eb-82e3-18a58805898f.png)
      >tampilan di Client apabila file tidak ada

      ![image](https://user-images.githubusercontent.com/43901559/118674123-c09fdb00-b823-11eb-975b-ec40da756f00.png)
      >tampilan di Client apabila Client ingin menghapus file yang telah dihapus
  * Rename file
    * jika ada file yang ingin dihapus, rename nama file
      ```c
      found = true;
      char old[] = "FILES/old-";
      strcat(old, temp_entry.name);
      rename(temp_entry.path, old);
      ```
  * Ubah `files.tsv`
    * selama membaca `files.tsv`, simpan pada baris ke-berapa terdapat detail dari file yang dihapus
    * buat fungsi untuk menghapus baris tertentu di `files.tsv`
      ```c
      removeLine(index);
      ```
      >panggil fungsi
    * fungsi merupakan modifikasi dari fungsi yang ditemukan di-web. Link tertera  
  
  #### Kendala Soal
  * tidak ada
  
  #### Referensi
  * fungsi remove line di file https://www.w3resource.com/c-programming-exercises/file-handling/c-file-handling-exercise-8.php
  
* ### f - get all files details
  #### Deskripsi Soal
  * Client mengirimkan command `see`
  * Format output di Client sebagai berikut:
    <pre>
    Nama:
    Publisher:
    Tahun publishing:
    Ekstensi File :
    Filepath :
    
    Nama:
    Publisher:
    Tahun publishing:
    Ekstensi File :
    Filepath :
    </pre>
  
  #### Solusi Soal
  * Client mengirimkan command `see`
    * Client-Server kirim-terima command
    * Server membuka `files.tsv`
  * Output di client
    * Server membaca `files.tsv` line-by-line
    * Server memproses tiap line agar mendapatkan informasi yang diperlukan
    * Selama masih ada baris untuk diproses, kirim informasi ke Client
      ```c
      char message[STR_SIZE];
      sprintf(message, "Nama : %s\nPublisher : %s\nTahun Publishing : %s\nEkstensi File : %s\nFilepath : %s\n\n", 
        temp_entry.name, temp_entry.publisher, temp_entry.year, ext, temp_entry.path);
                                
      send(new_socket, message, STR_SIZE, 0);
      ```
    * Client menerima informasi dari Server selama masih ada informasi untuk diterima
      
      ![image](https://user-images.githubusercontent.com/43901559/118676708-d31b1400-b825-11eb-96c6-1492ebfd5316.png)
      >tampilan di Client
  
  #### Kendala Soal
  * Kesulitan bagaimana client dapat menerima file selama masih ada file untuk diterima dan berhenti ketika tidak ada informasi untuk diterima
  
* ### g - WHERE LIKE
  #### Deskripsi Soal
  * Client menerima detail files yang mengandung string yang dikirimkan
  * Format output sama dengan 1.f
  
  #### Solusi Soal
  * Client send command
    * Client-Server kirim-terima
    * Server membaca `files.tsv` line-by-line
    * Manfaatkan strstr agar sesuai dengan apa yang diminta soal
      ```c
      if ((h = strstr(temp_entry.name, buffer)) != NULL)
      ```
  * Format output sama dengan 1.f
    * lihat penjelasan di 1.f 
  
  #### Kendala Soal
  * tidak paham cara pakai strstr

  #### Referensi
  * https://www.tutorialspoint.com/c_standard_library/c_function_strstr.htm
  
* ### h - log additions and deletions
  #### Deskripsi Soal
  * tiap operasi add dan delete, log di `running.log`
  * format `running.log`
    <pre>
    Tambah : File1.ektensi (id:pass)
    Hapus : File2.ektensi (id:pass)
    </pre>
  
  #### Solusi Soal
  * log tiap add dan delete
    * buat fungsi untuk memudahkan penambahan log di `running.log`
      ```c
      void log_action(char *type, char *fileName, char *user, char *pass) {
          FILE *log;
          char action[16];

          if (equal(type, "add")) {
              strcpy(action, "Tambah");
          }
          else if (equal(type, "delete")) {
              strcpy(action, "Hapus");
          }

          log = fopen("running.log", "a");
          fprintf(log, "%s : %s (%s:%s)\n", action, fileName, user, pass);
          fclose(log);

          return;
      }
      ```
      >fungsi menerima apa tipe operasinya (add/delete), file apa yang terpengaruh, siapa yang melakukannya beserta passwordnya
    * ketika Server memproses add dan delete, panggil fungsi tersebut
      ```c
      log_action("add", request.name, akun.name, akun.password);
      ```
      >panggil fungsi ketika terjadi file addition
      ```c
      log_action("delete", temp_entry.name, akun.name, akun.password);
      ```
      >panggil fungsi ketika terjadi deletion
  * format `running.log`
    * isi `running.log`
      
      ![image](https://user-images.githubusercontent.com/43901559/118678848-92bc9580-b827-11eb-8ea6-18bf6024d7c0.png)
      
  #### Kendala Soal
    * tidak ada

* ### appendix
  demi pemudahan pengerjaan soal, dibuat beberapa `struct`
  #### `Account`
    ```c
    typedef struct akun_t {
      char name[SIZE];
      char password[SIZE];
    } Account;
    ```
    >memudahkan login-register dan juga ketika log di 1.h
  
  #### `Entry`
    ```c
    typedef struct file_t {
      char publisher[SIZE];
      char year[SIZE];
      char name[SIZE];
      char path[SIZE];
    } Entry;
    ```
    >memudahkan semua operasi yang berhubungan dengan file terutama ketika string processing

## Soal 2

### Deskripsi Soal A

Membuat program perkalian matrix (4x3 dengan 3x6) dan menampilkan hasilnya.

### Jawaban Soal A

Pertama membuat tipe data baru yang kami beri nama `shared` dimana isinya ada 2 matrix yaitu data dan batas.
- Data adalah matrix yang akan menyimpan hasil perkalian dari ke 2 matrix yang akan dikalikan
- batas adalah matrix yang nanti akan berguna pada soal B, namun untuk saat ini masih belum digunakan

Tipe data `shared` inilah yang nantinya akan kami share menggunakan share memory agar nanti bisa dibuka pada soal B

Program utama pada soal A ini adalah program untuk mengalikan 2 matrix (4x3 dengan 3x6):
```c
void phrase_2a()
{
    unsigned long long int i, j, k;
    for(i = 0; i < 4; i++){
        for(j = 0; j < 6; j++){
            result[i][j] = 0;
            for(k = 0; k < 3; k++){
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
}
```
Algoritma yang kami gunakan adalah algoritma perkalian matrix seperti biasa seperti pada gambar dibawah ini:
![Perkalian-Dua-Matriks-2](https://user-images.githubusercontent.com/71221969/118937376-ad058900-b977-11eb-9a95-12b0c0c6ed25.png)

Setelah didapatkan matrix `result` maka saatnya menaruh matrix tersebut kedalam tipe data `shared` yang telah kami buat diatas lalu menggunakan shared memory selama 10 detik supaya Soal B dapat mengambil hasil perkalian dari matrix tersebut dalam rentang waktu 10 detik.
```c
    key_t key = 1244;
    int shmid = shmget(key,sizeof(struct shared),IPC_CREAT|0666);
    struct shared *shmptr;
    shmptr = (struct shared *) shmat(shmid, NULL, 0);

    for(i=0;i<4;i++){
        for(j=0;j<6;j++){
            shmptr->data[i][j] = result[i][j];
        }
        //printf("\n");
    }
    
    sleep(10);
    shmdt((void *) shmptr);
    shmctl(shmid, IPC_RMID, NULL);
```
Matrix yang akan dikalikan adalah matrix A dan matrix B yang telah kami definisikan di awal:
```c
unsigned long long int matrix1[4][3] = {{4, 1, 4}, {2, 1, 3}, {4, 2, 2}, {1, 1, 4}};
unsigned long long int matrix2[3][6] = {{2, 1, 3, 2, 0, 3}, {1, 4, 4, 0, 0, 2}, {1, 1, 0, 1, 2, 1}};
```
yang kemudian menghasilkan hasil seperti berikut ketika di run:
![Screenshot from 2021-05-20 14-43-24](https://user-images.githubusercontent.com/71221969/118939321-c0b1ef00-b979-11eb-9c84-550036b7abb5.png)


### Deskripsi Soal B

Membuat program dengan menggunakan matriks output dari program sebelumnya (program soal2a.c) emudian matriks tersebut akan dilakukan perhitungan dengan matrix baru (input user) sebagai berikut contoh perhitungan untuk matriks yang ada. Perhitungannya adalah setiap cel yang berasal dari matriks A menjadi angka untuk faktorial, lalu cel dari matriks B menjadi batas maksimal faktorialnya (dari paling besar ke paling kecil) dan menggunakan thread untuk perhitungan di setiap cel. apabila batas maksimal lebih besar dari angka yang akan difaktorialkan, maka faktorial akan berhenti pada 0!

### Jawaban Soal B

Pertama yang kami lakukan adalah membuat 2 tipe data baru yaitu `shared` untuk mendapatkan hasil perkalian matrix dari soal A dan `one_shared` yang nantinya untuk menampung 1 cell dari tipe data `shared`:
```c
struct shared{
    unsigned long long int data[4][6];
    unsigned long long int batas[4][6];
};
struct one_shared{
    unsigned long long int num;
    unsigned long long int limit;
};
```
Kedua, kami melakukan pengambilan matrix `shared` dari soal A dan menampungnya dalam `shared` pada soal B lalu membuat matrix degan nama `result` untuk menampung sementara matrix hasil perkalian yang didapat dari tipe data `shared` pada soal B
```c
    unsigned long long int result[4][6];
    key_t key = 1244;
    unsigned long long int shmid = shmget(key,sizeof(struct shared),0666);
    struct shared *shmptr;
    shmptr = (struct shared *) shmat(shmid, NULL, 0);

    memcpy(result, &shmptr->data, 24 * sizeof(unsigned long long int));
```
Ketiga, kami melakukan input untuk mengisi matrix `batas` dalam tipe data `shared` yang telah kami buat
```c
    printf("input matrix a yang merupakan batasan faktorial:\n");
    for(i=0;i<4;i++){
        for(j=0;j<6;j++){
            scanf("%lld",&shmptr->batas[i][j]);
        }
    }
```
dengan isi matrix `batas` sebagai berikut:
```
14 2 3 8 8 10
7 4 8 5 14 9
9 2 13 5 11 2
8 7 10 4 10 8
```
Keempat, kami membuat thread sebanyak 24 yang mana tiap thread akan melakukan operasi pada 1 cell pada matrix, disinilah tipe data `one_shared` berperan, dimana `one_shared` akan mengambil 1 cell pada `shared` lalu 1 thread akan mengambil 1 `one_shared` untuk melakukan operasinya
```c
    pthread_t tid[4][6];
	  for(int i = 0; i < 4;i++){
        for(int j=0;j<6;j++){
            struct one_shared *x =  malloc(sizeof(*x));
            if( x == NULL){
                printf("TIDAK BISA MEMBUAT RUANG\n");
                exit(1);
            }

            x->num = shmptr->data[i][j];
            x->limit = shmptr->batas[i][j];
            pthread_create(&(tid[i][j]), NULL, &factorial, x);
            sleep(1);
            pthread_join(tid[i][j], NULL);

        }	
        printf("\n");
	  }
```
## Soal 3

### Deskripsi Soal

Buatlah sebuah program C untuk mengategorikan file. Dalam kata lain, program C untuk memindahkan file-file ke dalam folder yang bernama ektensi dari file.

#### A

Program dapat menerima opsi `-f` untuk mengategorikan file. Misalnya, `./soal3.o -f file1.txt file2.jpeg file3.zip` berarti program akan memindahkan `file1.txt` ke dalam folder `txt`, memindahkan `file2.jpeg` ke dalam folder `jpeg`, dan memindahkan `file3.zip` ke dalam folder `san`.

Kondisi Awal Directory:

![image](https://user-images.githubusercontent.com/8071604/118130188-4cca9080-b427-11eb-90c1-e09ae956f562.png)

Running Program:

![image](https://user-images.githubusercontent.com/8071604/118130758-f1e56900-b427-11eb-8caf-ded4c4289d4a.png)

Kondisi Akhir Directory:

![image](https://user-images.githubusercontent.com/8071604/118131130-50aae280-b428-11eb-9e73-5aeb98e98c28.png)

![image](https://user-images.githubusercontent.com/8071604/118131152-599bb400-b428-11eb-8bdd-aa6588eadbef.png)

![image](https://user-images.githubusercontent.com/8071604/118131176-615b5880-b428-11eb-874f-33e469ac60b5.png)

![image](https://user-images.githubusercontent.com/8071604/118131195-68826680-b428-11eb-8f95-52c013e519df.png)

Strategi Penyelesaian:

Program dapat menerima opsi `-f`. Untuk mendeteksi opsi yang diberikan, argumen pada fungsi main digunakan.

```c
int main(int argc, char *argv[]) {
  return 0;
}
```

Dengan `argc` adalah jumlah argumen dan `argv` adalah array yang berisi argumen. Dalam kasus `./soal3.o -f file1.txt file2.jpeg file3.zip`, value dari `argc` dan `argv` adalah

```c
argc = 4
argv = {"-f", "file1.txt", "file2.jpeg", "file3.zip", NULL}
```

Pertama-tama, program mengecek apakah `argc > 1` dan apakah `strcmp(argv[0], "-f") == 0`. Jika ya, maka pindahkan file bernama `argv[1]`, `argv[2]`, ..., `argv[argc-1]`.

Proses pemindahannya dilakukan dengan menggunakan Thread. Setiap prosedur pemindahan satu file adalah satu Thread. Pada program C yang dibuat, ada fungsi `categorizeFile` yang akan melakukan prosedur pemindahan file ke dalam folder dengan nama ektensinya. Fungsi ini mengembalikan nilai 1 apabila proses pemindahan file berhasil. Jika tidak, fungsi ini akan mengembalikan nilai 0. Jika fungsi ini mengembalikan nilai 1, maka, sesuai dengan permintaan soal, teks "File 1 : Berhasil Dikategorikan (jika berhasil)" akan di-print di layar. Sebaliknya, jika fungsi ini mengembalikan nilai 0, maka teks "File 1: Sad gagal :(" akan di-print di layar.

Fungsi `categorizeFile` akan mengembalikan nilai 0 ketika file is not found.

#### B

Program dapat menerima opsi `-d` untuk mengategorikan file-file yang berada di dalam folder. Misalnya, `./soal3.o -d collection` berarti program akan memindahkan file-file yang berada di dalam folder `collection` ke dalam folder sesuai dengan nama ektensinya.

Kondisi Awal Direktori:

![image](https://user-images.githubusercontent.com/8071604/118132008-6967c800-b429-11eb-9c85-aea22b53889c.png)

![image](https://user-images.githubusercontent.com/8071604/118132034-71276c80-b429-11eb-8fea-40577cc94714.png)

Running Program:

![image](https://user-images.githubusercontent.com/8071604/118132118-88fef080-b429-11eb-9683-c066a6455a07.png)

Kondisi Akhir Direktori:

![image](https://user-images.githubusercontent.com/8071604/118132152-96b47600-b429-11eb-8dc7-63ece46d6e02.png)

![image](https://user-images.githubusercontent.com/8071604/118132180-9ddb8400-b429-11eb-9406-b176ac2ab1ef.png)

![image](https://user-images.githubusercontent.com/8071604/118132209-a59b2880-b429-11eb-80d5-7e6362dfb1a3.png)

![image](https://user-images.githubusercontent.com/8071604/118132242-acc23680-b429-11eb-9180-8d9b697b7a96.png)

![image](https://user-images.githubusercontent.com/8071604/118132278-b5b30800-b429-11eb-9464-198e5436a14b.png)

Strategi Penyelesaian:

Pengecekan terhadap argumen `-d` dilakukan sama seperti pada Poin A yaitu dengan mengecek apakah `argc > 1` dan `strcmp(argv[0], "-d") == 0`.

Setelah itu, directory yang dipilih, secara rekursif, di-scan. Setiap file yang ditemukan di dalam directory akan dipindahkan ke dalam folder sesuai dengan nama ektensinya. Setiap prosedur pemindahan satu file adalah satu Thread.

Pada program C ini, fungsi rekursif `recusrivelyCategorizeFile(basePath)` digunakan. Fungsi ini mengembalikan sebuah integer yang menyatakan banyaknya file yang berada di dalam directory basePath yang telah berhasil dipindahkan ke dalam folder sesuai dengan nama ektensinya. Jika fungsi ini mengembalikan nilai 0, maka hal ini berarti tidak ada file yang berhasil dipindahkan ke dalam folder sesuai dengan nama ektensinya. Oleh karena itu, jika fungsi ini mengembalikan nilai 0, maka, sesuai dengan permintaan soal, teks "Yah, gagal disimpan :(" di-print di layar. Sebaliknya, jika fungsi ini mengembalikan nilai setidaknya 1, maka teks "Berhasil Dikategorikan" akan di-print di layar.

#### C

Program menerima opsi `*` untuk mengategorikan seluruh file yang ada di working directory ketika program C dijalankan.

Kondisi Awal Direktori:

![image](https://user-images.githubusercontent.com/8071604/118132841-56092c80-b42a-11eb-8cca-1757e113bd29.png)

![image](https://user-images.githubusercontent.com/8071604/118132906-68836600-b42a-11eb-8010-957e569f470b.png)

Running Program:

![image](https://user-images.githubusercontent.com/8071604/118133049-8b157f00-b42a-11eb-8011-0aae9b5e2bec.png)

Kondisi Akhir Direktori:

![image](https://user-images.githubusercontent.com/8071604/118132979-7a650900-b42a-11eb-938f-14c82bc408fa.png)

Strategi Penyelesaian:

Pengecekan terhadap argumen `*` dilakukan dengan mengecek apakah `argc == 1` dan `strcmp(argv[0], "*") == 0)`.

Setelah itu, secara rekursif, working directory di-scan. Setiap file yang ditemukan di dalam working directory akan dipindahkan ke dalam folder sesuai dengan nama ektensinya. Dalam kasus ini, `soal3.c` dan `soal3.o` juga dipindahkan karena mereka berada di dalam working directory. Setelah program setelah melakukan tugasnya, directory c, jpeg, o, txt, dan zip terbentuk dan di dalamnya ada file yang mempunyai ektensi sama dengan nama foldernya.

#### D

File yang tidak mempunyai ektensi akan dipindahkan ke folder Unknown. File yang hidden akan dipindahkan ke folder Hidden.

File yang tidak mempunyai ektensi adalah file yang mempunyai nama ektensi. Nama ektensi dituliskan di akhir nama file. Nama ektensi mempunyai format .[nama ektensi]. Misalnya, .exe, .pdf, .zip, .c, .o, dan .jpeg.

File yang hidden adalah file yang mempunyai nama file dengan titik di awal nama filenya. Misalnya, .gitignore, .htaccess. Command untuk melihat hidden files adalah `ls - a`.

Berdasarkan sifat file tanpa ektensi dan file hidden yang telah diuraikan di atas, program C harus mengecek apakah file-file yang akan dipindahkan merupakan file tanpa ektensi atau file hidden atau file dengan ektensi.

![image](https://user-images.githubusercontent.com/8071604/118134636-59051c80-b42c-11eb-8d4d-9f21662d8651.png)

#### E

Setiap 1 file yang dikategorikan, dioperasikan oleh 1 Thread agar bisa berjalan secara paralel sehingga proses kategorisasi berjalan cepat.

Berikut ini adalah fungsi `categorizeFile` yang mengembalikan nilai 0 apabila proses kategorisasi file tidak berhasil dan mengembalikan nilai 1 apabila proses kategorisasi berhasil. Di dalam fungsi ini, setiap prosedur kategorisasi satu file dilakukan oleh satu Thread. Pada line 140, Thread dibuat dan memulai melakukan prosedur `__categorizeFile`.

```c
/*
  Categorize files (opening) by main thread.
*/
int categorizeFile(char *filePath) {
  // File does not exist
  if (!doesFileExist(filePath)) return 0;

  pthread_t tid;

  int err = pthread_create(&(tid), NULL, &(__categorizeFile), &filePath[0]);

  if (err != 0) {
    // There is error in creating thread
    exit(EXIT_FAILURE);
  }
  
  pthread_join(tid, NULL);
  return 1;
}
```


