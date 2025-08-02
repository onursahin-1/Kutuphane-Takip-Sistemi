#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

struct Book {
    int id;
    char title[100];
    char author[50];
    bool isBorrowed;
    char borrowDate[11]; // GG/AA/YYYY
};

// Kitap ekleme
void addBook() {
    Book book;
    ofstream file("books.txt", ios::app | ios::binary);

    if (!file) {
        cout << "Dosya acilamadi!\n";
        return;
    }

    cout << "\n--- Kitap Ekleme ---\n";

    cout << "Kitap ID: ";
    cin >> book.id;
    cin.ignore();

    while (true) {
        cout << "Kitap Adi: ";
        cin.getline(book.title, 100);
        if (strlen(book.title) == 0) {
            cout << "Bos giris yapilamaz! Lutfen tekrar girin.\n";
            continue;
        }
        break;
    }

    while (true) {
        cout << "Yazar Adi: ";
        cin.getline(book.author, 50);
        if (strlen(book.author) == 0) {
            cout << "Bos giris yapilamaz! Lutfen tekrar girin.\n";
            continue;
        }
        break;
    }

    book.isBorrowed = false;
    strcpy(book.borrowDate, "-"); // varsayilan

    file.write(reinterpret_cast<char*>(&book), sizeof(Book));
    file.close();

    cout << "Kitap basariyla eklendi.\n";
}

// Kitap listeleme
void listBooks() {
    Book book;
    ifstream file("books.txt", ios::binary);

    if (!file) {
        cout << "Dosya acilamadi!\n";
        return;
    }

    cout << "\n--- Kayitli Kitaplar ---\n";
    bool anyBook = false;

    while (file.read(reinterpret_cast<char*>(&book), sizeof(Book))) {
        anyBook = true;

        cout << "\n----------------------------\n";
        cout << "ID: " << book.id << endl;
        cout << "Kitap Adi: " << book.title << endl;
        cout << "Yazar: " << book.author << endl;
        cout << "Durum: " << (book.isBorrowed ? "Oduncte" : "Musait") << endl;
        if (book.isBorrowed)
            cout << "Odunc Alim Tarihi: " << book.borrowDate << endl;

        cout << "Kitap Adi (harf harf): ";
        for (int i = 0; book.title[i] != '\0'; i++) {
            cout << "[" << book.title[i] << "]";
        }
        cout << endl;
    }

    if (!anyBook) {
        cout << "Henuz hicbir kitap eklenmemis.\n";
    }

    file.close();
}

// Kitap arama
void searchBook() {
    Book book;
    ifstream file("books.txt", ios::binary);
    if (!file) {
        cout << "Dosya acilamadi!\n";
        return;
    }

    int choice;
search_menu:
    cout << "\n--- Kitap Arama ---\n";
    cout << "1. Kitap ID'sine Gore Ara\n";
    cout << "2. Kitap Adina Gore Ara\n";
    cout << "Seciminiz (1-2): ";
    cin >> choice;
    cin.ignore();

    bool found = false;

    switch (choice) {
        case 1: {
            int searchId;
            cout << "Aranacak Kitap ID: ";
            cin >> searchId;

            while (file.read(reinterpret_cast<char*>(&book), sizeof(Book))) {
                if (book.id == searchId) {
                    found = true;
                    cout << "\nKitap Bulundu!\n";
                    cout << "ID: " << book.id << endl;
                    cout << "Ad: " << book.title << endl;
                    cout << "Yazar: " << book.author << endl;
                    cout << "Durum: " << (book.isBorrowed ? "Oduncte" : "Musait") << endl;
                    if (book.isBorrowed)
                        cout << "Odunc Alim Tarihi: " << book.borrowDate << endl;
                    break;
                }
            }
            break;
        }
        case 2: {
            char searchTitle[100];
            cout << "Aranacak Kitap Adi: ";
            cin.getline(searchTitle, 100);

            while (file.read(reinterpret_cast<char*>(&book), sizeof(Book))) {
                if (strcmp(book.title, searchTitle) == 0) {
                    found = true;
                    cout << "\nKitap Bulundu!\n";
                    cout << "ID: " << book.id << endl;
                    cout << "Ad: " << book.title << endl;
                    cout << "Yazar: " << book.author << endl;
                    cout << "Durum: " << (book.isBorrowed ? "Oduncte" : "Musait") << endl;
                    if (book.isBorrowed)
                        cout << "Odunc Alim Tarihi: " << book.borrowDate << endl;
                    break;
                }
            }
            break;
        }
        default:
            cout << "Gecersiz secim! Menuye donuluyor.\n";
            goto search_menu;
    }

    if (!found) {
        cout << "Kitap bulunamadi.\n";
    }

    file.close();
}

// Kitap odunc alma
void borrowBook() {
    fstream file("books.txt", ios::in | ios::out | ios::binary);

    if (!file) {
        cout << "Dosya acilamadi!\n";
        return;
    }

    int borrowId;
    bool found = false;

    cout << "\n --- Kitap Odunc Alma ---\n";
    cout << "Odunc almak istediginiz kitap ID: ";
    cin >> borrowId;

    Book book;

    while (file.read(reinterpret_cast<char*>(&book), sizeof(Book))) {
        if (book.id == borrowId) {
            found = true;

            if (!book.isBorrowed) {
                book.isBorrowed = true;
                cout << "Tarih girin (GG/AA/YYYY): ";
                cin.ignore();
                cin.getline(book.borrowDate, 11);
                file.seekp(-static_cast<int>(sizeof(Book)), ios::cur);
                file.write(reinterpret_cast<char*>(&book), sizeof(Book));
                cout << "Kitap basariyla odunc alindi.\n";
            } else {
                cout << "Bu kitap zaten odunc alinmis.\n";
            }

            break;
        }
    }

    if (!found) {
        cout << "Kitap bulunamadi.\n";
    }

    file.close();
}

// Kitap iade etme
void returnBook() {
    fstream file("books.txt", ios::in | ios::out | ios::binary);

    if (!file) {
        cout << "Dosya acilamadi!\n";
        return;
    }

    int returnId;
    bool found = false;

    cout << "\n --- Kitap Iade Etme ---\n";
    cout << "Iade etmek istediginiz kitap ID: ";
    cin >> returnId;

    Book book;

    while (file.read(reinterpret_cast<char*>(&book), sizeof(Book))) {
        if (book.id == returnId) {
            found = true;

            if (book.isBorrowed) {
                book.isBorrowed = false;
                strcpy(book.borrowDate, "-");
                file.seekp(-static_cast<int>(sizeof(Book)), ios::cur);
                file.write(reinterpret_cast<char*>(&book), sizeof(Book));
                cout << "Kitap basariyla iade edildi.\n";
            } else {
                cout << "Bu kitap zaten kutuphanede.\n";
            }

            break;
        }
    }

    if (!found) {
        cout << "Kitap bulunamadi.\n";
    }

    file.close();
}

// Kitap silme
void deleteBook() {
    int deleteId;
    bool found = false;

    cout << "\n--- Kitap Silme ---\n";
    cout << "Silmek istediginiz kitap ID: ";
    cin >> deleteId;

    ifstream infile("books.txt", ios::binary);
    ofstream tempFile("temp.txt", ios::binary);

    if (!infile || !tempFile) {
        cout << "Dosya acilamadi!\n";
        return;
    }

    Book book;
    while (infile.read(reinterpret_cast<char*>(&book), sizeof(Book))) {
        if (book.id == deleteId) {
            found = true;
            continue;
        }
        tempFile.write(reinterpret_cast<char*>(&book), sizeof(Book));
    }

    infile.close();
    tempFile.close();

    remove("books.txt");
    rename("temp.txt", "books.txt");

    if (found)
        cout << "Kitap basariyla silindi.\n";
    else
        cout << "Kitap bulunamadi.\n";
}

// Ana menu
void menu() {
    int choice;

    while (true) {
        cout << "\n ----- KUTUPHANE SISTEMI -----\n";
        cout << "1. Kitap Ekle\n";
        cout << "2. Kitaplari Listele\n";
        cout << "3. Kitap Ara\n";
        cout << "4. Kitap Odunc Al\n";
        cout << "5. Kitap Iade Et\n";
        cout << "6. Kitap Sil\n";
        cout << "7. Cikis\n";
        cout << "Seciminiz: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: addBook(); break;
            case 2: listBooks(); break;
            case 3: searchBook(); break;
            case 4: borrowBook(); break;
            case 5: returnBook(); break;
            case 6: deleteBook(); break;
            case 7:
                cout << "Programdan cikiliyor...\n";
                return;
            default:
                cout << "Gecersiz secim! Lutfen 1-7 arasinda bir deger girin.\n";
        }
    }
}

// Ana fonksiyon
int main() {
    menu();
    return 0;
}
