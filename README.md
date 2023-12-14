# Final Project 
### Struktur Data Kelas A
### Cafe Order System

---
**Kelompok 13**
|Nama|NRP  |
|--|--|
|Iki Adfi Nur Mohamad|5027221033|
|Bintang Ryan Wardana|5027221036|
|Zidny Ilman Nafian|5027221039|

---
**A. PEMBAHASAN**
1. Pertama membuat fungsi untuk membuat cleaner.log, `buatlog` adalah fungsi yang digunakan untuk mencatat informasi tentang file yang dihapus ke dalam file .log. Fungsi ini menerima dua parameter: `path` dan `filename` yang merepresentasikan directory lengkap file yang dihapus. Fungsi ini menggunakan `time` untuk mendapatkan waktu saat ini dan `strftime` untuk mengonversi waktu menjadi format yang sesuai. Fungsi ini membuka file log /home/adfi/cleaner.log dalam mode tambahan ("a"), menulis informasi log, dan kemudian menutup file.

```c
void buatlog(const char *path, const char *filename) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char timestamp[20];
    strftime(timestamp, 20, "%Y-%m-%d %H:%M:%S", tm_info);

    FILE *log_file = fopen("/home/adfi/cleaner.log", "a");
    if (log_file) {
        fprintf(log_file, "[%s] '%s/%s' has been removed.\n", timestamp, path, filename);
        fclose(log_file);
    }
}
```

2. Fungsi `carisuspicious` digunakan untuk mencari kata "SUSPICIOUS" dalam sebuah file. Fungsi ini menerima parameter `filename` yang adalah nama file yang akan diperiksa. Fungsi membuka file tersebut, membaca baris per baris, dan mencari kata "SUSPICIOUS" dalam setiap baris. Jika kata "SUSPICIOUS" ditemukan, maka fungsi akan mengembalikan 1; jika tidak ditemukan, maka akan mengembalikan 0.

```c
int carisuspicious(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return 0;
    }

    char line[1000];
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "SUSPICIOUS")) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}
```

3. Fungsi `folderbersih` ini digunakan untuk memeriksa apakah sebuah direktori bersih dari file yang mengandung kata "SUSPICIOUS" atau tidak. Fungsi ini menerima parameter `path`, yang adalah alamat direktori yang akan diperiksa. Fungsi membuka direktori tersebut dan memeriksa setiap file di dalamnya menggunakan fungsi `carisuspicious`. Jika ada file yang mengandung kata "SUSPICIOUS", variabel `hasSuspicious` akan diatur menjadi 1. Fungsi ini mengembalikan 1 jika direktori bersih (tidak ada file "SUSPICIOUS"), dan 0 jika tidak bersih.

```c
int folderbersih(const char *path) {
    struct dirent *entry;
    DIR *dir = opendir(path);
    if (!dir) {
        return 1;
    }

    int hasSuspicious = 0; // Tambahkan variabel untuk melacak adanya SUSPICIOUS dalam direktori

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            if (carisuspicious(entry->d_name)) {
                hasSuspicious = 1;
            }
        }
    }

    closedir(dir);

    return !hasSuspicious; // Mengembalikan 1 jika tidak ada SUSPICIOUS
}
```


4. FungSi `hapussuspicious` adalah fungsi utama yang digunakan untuk menghapus file yang mengandung kata "SUSPICIOUS" dalam sebuah direktori. Fungsi ini menerima parameter `path`, yang adalah alamat direktori yang akan diproses. Fungsi ini membuka direktori tersebut, mengakses setiap entri (file atau subdirektori), dan menghapus file jika mengandung kata "SUSPICIOUS". Fungsi juga melakukan rekursi ke dalam subdirektori untuk melakukan pemeriksaan dan penghapusan. Fungsi ini juga mencatat informasi penghapusan menggunakan fungsi `buatlog`. Terdapat juga pemanggilan `sleep(30)` untuk memberi jeda 30 detik setiap kali sebuah file "SUSPICIOUS" dihapus. Fungsi ini kemudian kembali ke direktori sebelumnya menggunakan `chdir("..")` dan menutup direktori yang sedang diproses.

```c
void hapussuspicious(const char *path) {
    struct dirent *entry;
    struct stat statbuf;

    DIR *dir = opendir(path);
    if (!dir) {
        return;
    }

    chdir(path);  // Pindah ke direktori yang akan dicari SUSPICIOUS nya

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        if (lstat(entry->d_name, &statbuf) == -1) {
            continue;
        }

        if (S_ISDIR(statbuf.st_mode)) {
            hapussuspicious(entry->d_name);
        } else if (S_ISREG(statbuf.st_mode) && carisuspicious(entry->d_name)) {
            remove(entry->d_name);
            buatlog(path, entry->d_name);
        }
        sleep(30);
    }

    chdir("..");  // Kembali ke direktori sebelumnya
    closedir(dir);
}
```

5. Fungsi `main` adalah fungsi utama yang akan dijalankan saat program dimulai. Fungsi ini menggunakan `fork()` untuk menciptakan proses baru. Proses child akan menjadi daemon yang menjalankan operasi pembersihan. Proses parent/utama akan segera keluar setelah proses child dibuat. Fungsi ini menjalankan `hapussuspicious` dan `folderbersih` dalam loop hingga seluruh direktori bersih dari file "SUSPICIOUS". Program ini terus berjalan sebagai daemon sampai semua direktori bersih dari "SUSPICIOUS".

```c
int main(int argc, char *argv[]) {

    pid_t pid, sid;
    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0);
    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    if (chdir("/") < 0) {
        exit(EXIT_FAILURE);
    }

    while (1) {
        hapussuspicious(argv[1]);
        if (folderbersih(argv[1])) {
            break; // Keluar dari loop jika tidak ada SUSPICIOUS lagi
        }
    }

    return 0;
}
```

6. Program diatur agar otomatis berhenti ketika tidak ada lagi file “SUSPICIOUS”

```c
while (1) {
        hapussuspicious(argv[1]);
        if (folderbersih(argv[1])) {
            break; // Keluar dari loop jika tidak ada SUSPICIOUS lagi
        }
    }
```


**B. HASIL**

- Susunan directory sebelum program jalan

![Screenshot_2023-10-12_065825](/uploads/485586cadf324793a8a969a8953c3410/Screenshot_2023-10-12_065825.png)

![Screenshot_2023-10-12_065727](/uploads/74ae9fcf09a62728ae8415bb36f19cc5/Screenshot_2023-10-12_065727.png)


- Susunan directory setelah program jalan

![Screenshot_2023-10-12_070719](/uploads/96a7bd05df5e7b547d87d7531a45a998/Screenshot_2023-10-12_070719.png)

![Screenshot_2023-10-12_070739](/uploads/093cfb307650f6a128151e8f795d03f0/Screenshot_2023-10-12_070739.png)

- cleaner.log

![Screenshot_2023-10-12_070550](/uploads/6048f84d2a22db5be77b6d02ca07aae1/Screenshot_2023-10-12_070550.png)


- Letak cleaner; cleaner.c; cleaner.log di /home/user

![Screenshot_2023-10-12_070653](/uploads/ae85d3703959954b89761d884a1e5a02/Screenshot_2023-10-12_070653.png)
