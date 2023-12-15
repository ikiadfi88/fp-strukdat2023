#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>
#include <unordered_map>
#include <list>
#include <queue>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <thread>

using namespace std;

class MenuComponent {
public:
    virtual void infoMamin() const = 0;
    virtual double infoHarga() const = 0;
    virtual ~MenuComponent() {}
};

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


class CafePesenSystem {
private:
    vector<MenuComponent*> orders;
    queue<Pesen*> orderQueue;
    int orderCounter;
    MenuGraph menuGraph;

public:
    const MenuGraph& getMenuGraph() const {
        return menuGraph;
    }

    CafePesenSystem() : orderCounter(1) {}

    ~CafePesenSystem() {
        for (MenuComponent* pesen : orders) {
            delete pesen;
        }
    }

    void createPesen() {
        Pesen* pesen = new Pesen(orderCounter++);
        int choice;
        bool addMoreItems = true;

        while (addMoreItems) {
            int itemId;
            cout << "Pilihan Menu:" << endl;
            cout << "1. Nasi Goreng - Rp20.000" << endl;
            cout << "2. Macha Latte - Rp18.000" << endl;
            cout << "3. French Fries - Rp10.000" << endl;
            cout << "4. Americano - Rp10.000" << endl;
            cout << "5. Milk Coffee - Rp18.000" << endl;
            cout << "6. Ice Tea - Rp10.000" << endl;
            cout << "7. Beef Teriyaki - Rp25.000" << endl;
            cout << "8. Ice Chocolate - Rp18.000" << endl;
            cout << "9. Chicken Katsu - Rp23.000" << endl;
            cout << "10. Dori Rice Bowl - Rp28.000" << endl;
            cout << "11. Noodles Egg Premium - Rp18.000" << endl;
            cout << "12. Milk Tea - Rp13.000" << endl;
            cout << "13. Vietnam Drip - Rp18.000" << endl;
            cout << "14. Mineral Water - Rp6.000" << endl;
            cout << "15. Dimsum - Rp15.000" << endl;

            cout << "Masukkan ID item yang ingin dipesan (1-15): ";
            cin >> itemId;

            if (itemId >= 1 && itemId <= 15) {
                MenuItem* newItem = new MenuItem(itemId, getNamaMenu(itemId), getMenuHarga(itemId));
                pesen->addItem(newItem);
                menuGraph.addEdge(pesen, newItem, pesen->getPesenId());
            } else {
                cout << "ID item tidak valid. Silakan pilih ID item yang valid." << endl;
            }

            char addMore;
            cout << "Tambah item lagi? (y/n): ";
            cin >> addMore;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (addMore != 'y' && addMore != 'Y') {
                addMoreItems = false;
            }
        }

        orders.push_back(pesen);
        orderQueue.push(pesen);
        cout << "Pesanan berhasil dibuat. ID Pesanan: " << pesen->getPesenId() << endl;
    }

    void infoMaminPesensQueue() const {
        if (!orderQueue.empty()) {
        cout << "\nAntrian Pesanan:" << endl;
        queue<Pesen*> tempQueue = orderQueue;
        while (!tempQueue.empty()) {
            tempQueue.front()->infoMamin();
            tempQueue.pop();
        }
    } else {
    }
    }

    void infoMaminPesens() const {
        if (orders.empty() && orderQueue.empty()) {
            cout << "Belum ada pesanan. Silakan tambahkan pesanan terlebih dahulu." << endl;
        } else {
            cout << "\nDaftar Pesanan:" << endl;
            for (const MenuComponent* pesen : orders) {
                pesen->infoMamin();
                cout << "Total Harga: Rp" << fixed << setprecision(3) << pesen->infoHarga() << endl;
            }
        }
    }


    int getPesenCounter() const {
        return orderCounter;
    }

    string getNamaMenu(int itemId) const {
        switch (itemId) {
            case 1:
                return "Nasi Goreng";
            case 2:
                return "Macha Latte";
            case 3:
                return "French Fries";
            case 4:
                return "Americano";
            case 5:
                return "Milk Tea";
            case 6:
                return "Ice Tea";
            case 7:
                return "Beef Teriyaki";
            case 8:
                return "Ice Chocolate";
            case 9:
                return "Chicken Katsu";
            case 10:
                return "Dori Rice Bowl";
            case 11:
                return "Noodles Egg Premium";
            case 12:
                return "Milk Tea";
            case 13:
                return "Vietnam Drip";
            case 14:
                return "Mineral Water";
            case 15:
                return "Dimsum";
            default:
                return "Unknown";
        }
    }

    double getMenuHarga(int itemId) const {
        switch (itemId) {
            case 1:
                return 20.000;
            case 2:
                return 18.000;
            case 3:
                return 10.000;
            case 4:
                return 10.000;
            case 5:
                return 18.000;
            case 6:
                return 10.000;
            case 7:
                return 25.000;
            case 8:
                return 18.000;
            case 9:
                return 23.000;
            case 10:
                return 28.000;
            case 11:
                return 18.000;
            case 12:
                return 13.000;
            case 13:
                return 18.000;
            case 14:
                return 6.000;
            case 15:
                return 15.000;
            default:
                return 0.0;
        }
    }

    void pesenanDiproses() {
        if (orderQueue.empty()) {
            cout << "Belum ada pesanan untuk diproses." << endl;
            return;
        }

        while (!orderQueue.empty()) {
            Pesen* currentPesen = orderQueue.front();
            orderQueue.pop();

            cout << "Memproses Pesanan ID " << currentPesen->getPesenId() << "..." << endl;

            size_t itemCount = currentPesen->getItemCount();
            int processingTime = itemCount * 2;

            this_thread::sleep_for(chrono::seconds(processingTime));

            cout << "Pesanan ID " << currentPesen->getPesenId() << " selesai diproses." << endl;
        }

        // ini buat hapus pesenan setelah diproses
        for (MenuComponent* pesen : orders) {
            delete pesen;
        }
        orders.clear();
        while (!orderQueue.empty()) {
            orderQueue.pop();
        }
    }



};

int main() {
    CafePesenSystem cafeSystem;

    int choice;
    bool exitMenu = false;

    while (!exitMenu) {
        cout << "\n===== Cafe Pesen System =====\n" << endl;
        cout << "1. Tambah Pesanan" << endl;
        cout << "2. Tampilkan Pesanan" << endl;
        cout << "3. Proses Pesanan" << endl;
        cout << "4. Exit" << endl;
        cout << "Masukkan pilihan Anda: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: {
                cafeSystem.createPesen();
                break;
            }
            case 2: {
                if (cafeSystem.getPesenCounter() > 1) {
                    cafeSystem.infoMaminPesens();
                    cafeSystem.infoMaminPesensQueue();
                } else {
                    cout << "Belum ada pesanan. Silakan tambahkan pesanan terlebih dahulu." << endl;
                }
                break;
            }
            case 3: {
                cafeSystem.pesenanDiproses();
                break;
            }
            case 4: {
                cout << "Keluar dari program. Selamat tinggal!" << endl;
                exitMenu = true;
                break;
            }
            default: {
                cout << "Pilihan tidak valid. Masukkan pilihan yang valid." << endl;
                break;
            }
        }
    }

    return 0;
}