# soal-shift-sisop-modul-3-A05-2021

## Soal 1

## Soal 2

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

Proses pemindahannya dilakukan dengan menggunakan Thread. Setiap prosedur pemindahan satu file adalah satu Thread.

#### B

Program dapat menerima opsi `-d` untuk mengategorikan file-file yang berada di dalam folder. Misalnya, `./soal3.o -d arigatou` berarti program akan memindahkan file-file yang berada di dalam folder `arigatou` ke dalam folder sesuai dengan nama ektensinya.

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


