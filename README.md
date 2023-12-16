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

**A. PROBLEM**  
    Program Cafe Order System dirancang untuk menangani permasalahan manajemen pesanan di sebuah kafe. Dengan menyediakan fungsionalitas seperti penambahan pesanan baru, antrian pesanan, dan pemrosesan pesanan, program ini memungkinkan kafe untuk secara efisien mengelola pesanan pelanggan. Manajemen menu yang terintegrasi memungkinkan identifikasi hubungan antara pesanan dan item-menu, sementara antarmuka pengguna sederhana memberikan kemudahan dalam penggunaan program. Dengan implementasi ini, program memberikan solusi terstruktur untuk meningkatkan efisiensi dan akurasi dalam operasional sehari-hari kafe.
    
**A. OVERVIEW PROGRAM**  
    Program Cafe Order System ini merupakan implementasi sederhana untuk manajemen pesanan di sebuah kafe. Program ini menggunakan konsep hierarki kelas dengan menggunakan pola desain komposit untuk merepresentasikan menu dan pesanan. Program juga memiliki struktur data antrian untuk mengelola pesanan yang harus diproses. Program memberikan fungsionalitas seperti penambahan pesanan, tampilan daftar pesanan, tampilan antrian pesanan, dan pemrosesan pesanan. Antarmuka pengguna sederhana diberikan melalui konsol dengan opsi untuk menambah pesanan, menampilkan pesanan, memproses pesanan, dan keluar dari program. Adapun hal-hal yang diimplementasikan pada program ini yaitu :  
    1. Implementasi Class, Property, Method, Constructor, Destructor  
    2. Implementasi Adjacency list / Adjacency Matrix  
    3. Instansiasi Class  
    4. Implementasi Abstract Class  
    5. Implementasi Encapsulation: Public / Private / Protected  
    6. Implementasi Inheritance: Single / Hierarchical / Multilevel
    7. Implementasi Polymorphism: Overriding dan Overloading
    8. Implementasi Composition  
    
    
```c++
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

```c++
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

```c++
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

```c++
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

```c++
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

```c++
while (1) {
        hapussuspicious(argv[1]);
        if (folderbersih(argv[1])) {
            break; // Keluar dari loop jika tidak ada SUSPICIOUS lagi
        }
    }
```

7. Polimorfisme adalah konsep dalam pemrograman berorientasi objek di mana suatu objek dapat berperilaku sebagai objek dari kelas lain melalui warisan dan penggantian metode. Pada kode kami terdapat penggunaan overriding sebagai berikut:

A. Class `MenuItem`

Metode `infoMamin() const override` ini adalah contoh overriding. Metode ini diambil dari kelas induk `(MenuComponent)` dan dideklarasikan kembali di kelas turunan `(MenuItem)`. Metode ini menggantikan metode yang sama dari kelas induk. Dalam kasus ini, `infoMamin()` memberikan informasi tentang item menu, dan implementasinya disesuaikan untuk kelas `MenuItem`. Metode `infoHarga() const override` ini juga merupakan overriding. Metode ini diambil dari kelas induk dan dideklarasikan kembali di kelas `MenuItem`. Fungsinya adalah mengembalikan harga item menu.

```c++
    class MenuItem : public MenuComponent {
    private:
        int itemId;
        string itemNama;
        double itemHarga;
    
    public:
        MenuItem(int id, string name, double harga) : itemId(id), itemNama(name), itemHarga(harga) {}
    
        void infoMamin() const override {
            cout << "- " << itemNama << " (ID: " << itemId << "), Harga: Rp" << fixed << setprecision(3) << itemHarga << endl;
        }
    
        double infoHarga() const override {
            return itemHarga;
        }
    };
```

B. Class `Pesen`

Metode `infoMamin() const override` seperti pada kelas `MenuItem`, metode ini overriding metode yang sama dari kelas induk. Metode ini memberikan informasi tentang pesanan, termasuk ID pesanan, timestamp, dan item-item yang ada dalam pesanan. Setiap item di pesanan memanggil metode `infoMamin()`-nya sendiri. Metode `infoHarga() const override` juga merupakan overriding. Metode ini menghitung dan mengembalikan total harga dari semua item dalam pesanan dengan menjalankan `infoHarga()` pada setiap item.Metode getItemCount() const: Metode ini tidak ada di kelas induk `(MenuComponent)`. Ini adalah contoh penambahan metode yang tidak ada di kelas induk, menunjukkan bahwa program dapat menambahkan fungsionalitas tambahan pada kelas turunan tanpa mempengaruhi kelas induk.

```c++
    class Pesen : public MenuComponent {
    private:
        int orderId;
        time_t timestamp;
        vector<MenuComponent*> items;
    
    public:
        Pesen(int id) : orderId(id), timestamp(time(nullptr)) {}
    
        int getPesenId() const {
            return orderId;
        }
    
        const time_t getTimestamp() const {
            return timestamp;
        }
    
        void addItem(MenuComponent* item) {
            items.push_back(item);
        }
    
        void infoMamin() const override {
            struct tm* localTime = localtime(&timestamp);
            cout << "\nPesen ID: " << orderId << ", Timestamp: " << put_time(localTime, "%Y-%m-%d %H:%M:%S") << endl;
            cout << "Items:" << endl;
            for (const MenuComponent* item : items) {
                item->infoMamin();
            }
            cout << endl;
        }
    
        double infoHarga() const override {
            double total = 0.0;
            for (const MenuComponent* item : items) {
                total += item->infoHarga();
            }
            return total;
        }
    
        size_t getItemCount() const {
            return items.size();
        }
    };
```

Terdapat dua versi dari fungsi addEdge pada class `MenuGraph`. Kedua versi tersebut menunjukkan implementasi overloading pada level fungsi:
 
A. Fungsi addEdge dengan Tiga Parameter

```c++
void addEdge(MenuComponent* menu1, MenuComponent* menu2, int weight)
```

Fungsi ini digunakan untuk menambahkan edge antara dua objek `MenuComponent` dan memberikan bobot (weight) yang diberikan. Ini adalah bentuk dasar dari fungsi `addEdge` yang menerima tiga parameter, yaitu dua objek `MenuComponent` dan sebuah bobot. Dalam kasus ini, edge diimplementasikan dalam bentuk adjacency list dengan bobot yang ditentukan.

B. Fungsi addEdge dengan Dua Parameter

```c++
void addEdge(MenuComponent* menu1, MenuComponent* menu2)
```
Fungsi ini juga merupakan versi dari `addEdge`, tetapi kali ini hanya menerima dua objek `MenuComponent` tanpa menyertakan bobot. Dalam hal ini, fungsi menetapkan bobot default untuk edge yang ditambahkan antara dua objek. Ini memberikan fleksibilitas untuk menambahkan edge tanpa harus menyertakan bobot secara eksplisit.

Dengan adanya kedua versi fungsi addEdge ini, kita dapat dengan mudah menambahkan edge dengan atau tanpa bobot sesuai dengan kebutuhan kita. Dalam konteks ini, overloading memungkinkan kita menggunakan nama fungsi yang sama untuk tugas yang mirip tetapi dengan perbedaan dalam jumlah atau tipe parameter.

```c++
class MenuGraph {
private:
    std::unordered_map<MenuComponent*, std::unordered_map<MenuComponent*, int>> adjacencyList;

public:
    void addEdge(MenuComponent* menu1, MenuComponent* menu2, int weight) {
        adjacencyList[menu1][menu2] = weight;
        adjacencyList[menu2][menu1] = weight;
    }

    void addEdge(MenuComponent* menu1, MenuComponent* menu2) {
        const int defaultWeight = 1;
        adjacencyList[menu1][menu2] = defaultWeight;
        adjacencyList[menu2][menu1] = defaultWeight;
    }
};
```
    

8. Implementasi composition pada kode tersebut terjadi dalam class `Pesen`, di mana `Pesen` memiliki objek bertipe `vector<MenuComponent*> items`. Di dalam class `Pesen`, kita mendeklarasikan sebuah vektor (`items`) yang menyimpan pointer ke objek dari tipe `MenuComponent`. Ini adalah contoh composition di mana class `Pesen` memiliki komponen berupa kumpulan objek `MenuComponent`.

Dengan metode `addItem`, class `Pesen` memberikan fungsionalitas untuk menambahkan objek bertipe `MenuComponent` ke dalam vektor `items`. Ini merupakan contoh composition dalam tindakan, di mana objek `Pesen` berinteraksi dengan objek `MenuComponent`.

```c++
void addItem(MenuComponent* item) {
    items.push_back(item);
}
```

Dalam metode `infoMamin`, class `Pesen` menggunakan komponen `items` dengan mengiterasi melalui vektor tersebut. Untuk setiap elemen di dalam vektor, dipanggil metode `infoMamin()` dari objek `MenuComponent`. Ini merupakan contoh composition di mana objek `Pesen` menggunakan dan mengintegrasikan objek-objek dari tipe `MenuComponent` ke dalam fungsionalitasnya sendiri.

```c++
for (const MenuComponent* item : items) {
    item->infoMamin();
}
```

Dalam metode `infoHarga`, class `Pesen` menggunakan komponen `items` untuk menghitung total harga dengan menjumlahkan harga dari setiap objek `MenuComponent`. Ini merupakan contoh composition dalam mengumpulkan dan menggunakan informasi dari komponen-komponennya.

```c++
double total = 0.0;
for (const MenuComponent* item : items) {
    total += item->infoHarga();
}
```

Selain itu, implementasi composition juga ada pada kode `CafePesenSystem` terjadi dalam beberapa aspek, terutama melalui hubungan antara `CafePesenSystem` dan objek-objek dari class lain, seperti `Pesen`, `MenuComponent`, dan `MenuGraph`. Dalam class `CafePesenSystem`, kita memiliki vektor orders yang menyimpan pointer ke objek-objek dari tipe `MenuComponent`. Hal ini menciptakan hubungan composition, di mana `CafePesenSystem` memiliki komponen bertipe `MenuComponent` dalam bentuk pesanan (`Pesen`).


```c++
vector<MenuComponent*> orders;
```

Vektor `orderQueue` digunakan untuk menyimpan pointer ke objek-objek `Pesen`, yang juga merupakan objek dari tipe `MenuComponent`. Ini menunjukkan hubungan composition antara `CafePesenSystem` dan objek `Pesen`, di mana `CafePesenSystem` memiliki dan mengelola pesanan dalam antrian.

```c++
queue<Pesen*> orderQueue;
```

Objek `menuGraph` merupakan instance dari class `MenuGraph`, yang dijadikan sebagai komponen dari `CafePesenSystem`. Hubungan composition terjadi di sini, di mana `CafePesenSystem` memiliki suatu grafik menu (`MenuGraph`) yang berhubungan dengan objek `MenuComponent`.

```c++
MenuGraph menuGraph;
```

Dalam metode `createPesen`, `CafePesenSystem` membuat pesanan (`Pesen`) dan menggunakan metode `addItem` dari objek Pesen untuk menambahkan objek `MenuItem` ke dalam pesanan. Selain itu, objek `CafePesenSystem` juga memanipulasi objek `MenuGraph` dengan menggunakan metode `addEdge` untuk menambahkan hubungan antara pesanan dan item menu dalam grafik.

```c++
void createPesen() {
    // ...
    MenuItem* newItem = new MenuItem(itemId, getNamaMenu(itemId), getMenuHarga(itemId));
    pesen->addItem(newItem);
    menuGraph.addEdge(pesen, newItem, pesen->getPesenId());
    // ...
}
```

Pada destruktor, `CafePesenSystem` membersihkan dan menghapus objek-objek pesanan (`MenuComponent`) yang disimpan dalam vektor `orders`. Ini adalah contoh dari composition di mana objek memiliki tanggung jawab untuk mengelola siklus hidup objek-objek yang dimilikinya.

```c++
~CafePesenSystem() {
    for (MenuComponent* pesen : orders) {
        delete pesen;
    }
}
```

**B. HASIL**

- Tampilan program saat pertama kali dijalankan

![awal mula](https://github.com/ikiadfi88/fp-strukdat2023/assets/120791817/448cd2d1-2b81-4931-bdc2-5b64883354b3)

- Tampilan program pada menu nomor 1

![awal pg 1](https://github.com/ikiadfi88/fp-strukdat2023/assets/120791817/40a6b49f-65e4-433a-98d3-4d11c47f047f)

- Tampilan program pada menu nomor 2 jika tidak ada pesanan

![awal pg 2](https://github.com/ikiadfi88/fp-strukdat2023/assets/120791817/68a6ea32-8411-4bae-a016-37063e09bdf1)

- Tampilan program noor 2 jika ada pesanan

![pg 2](https://github.com/ikiadfi88/fp-strukdat2023/assets/120791817/590e1ff7-68f1-4b42-98c3-3fa26fd62f2b)

![pg 2 b](https://github.com/ikiadfi88/fp-strukdat2023/assets/120791817/d30a93ad-a91c-49f2-aa45-ffde92431453)

- Tampilan program pada menu nomor 3 jika tidak ada pesanan

![awal pg 3](https://github.com/ikiadfi88/fp-strukdat2023/assets/120791817/d3f2a546-8e35-431d-b0db-e65fa095a6e0)

- Tampilan program pada menu nomor 2 jika ada pesanan

![pg 3](https://github.com/ikiadfi88/fp-strukdat2023/assets/120791817/e33eda29-dfad-4aea-bb1f-e81e39d6dcf6)

- Tampilan program pada menu nomor 4

![pg 4](https://github.com/ikiadfi88/fp-strukdat2023/assets/120791817/22493ecb-185f-4097-8886-746059d560b6)
