#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;

struct task {
    int id;
    string description;
    bool isComplated;
    
    string getstatus() const{
        return isComplated? "TAMAMLANDI" : "BEKLEMEDE";
    }
};

class taskmanager{
private:
    vector<task> tasks;
    int nextid = 1;
public:
    void addtask(const string& description){
        tasks.push_back({nextid++, description, false});
        cout << "Görev eklendi (ID: " << nextid-1 << "):" << description << endl;
    }
    
    void viewtasks()const{
        if (tasks.empty()){
            cout << "\n-----Liste Boş-----\n";
            return;
        }
        cout << "\n-----Güncel Görev Listesi-----" << endl;
        for(const auto& task : tasks){
            cout << " ID: " << task.id
            << " | " << task.getstatus()
            << " | Görev:" << task.description << endl;
        }
        cout << "---------------------------------\n" << endl;
    }
    
    void marktaskcomplated(int id){
        auto it = find_if(tasks.begin(), tasks.end(), [id](const task& t){return t.id ==id;});
        if (it != tasks.end()){
            if(!it->isComplated){
                it->isComplated = true;
                cout << "Görev " << id << " tamamlandı olarak işaretlendi." << endl;
            } else{
                cout << "Uyarı: Görev " << id << " zaten tamamlanmış durumda." << endl;
            }
        } else {
            cout << "HATA: Belirtilen ID (" << id << ") ile bir görev bulunamadı." << endl;
        }
    }
    
    void removetask(int id){
        size_t oldsize = tasks.size();
        auto newend = remove_if(tasks.begin(), tasks.end(), [id](const task& t){return t.id == id;});
        tasks.erase(newend, tasks.end());
        
        if(tasks.size() < oldsize){
            cout << "Görev " << id << " başarıyla silindi." << endl;
        } else{
            cout << "HATA: Belirtilen ID (" << id << ") bulunamadı." << endl;
        }
    }
    
    
    void loadfromfile(){
        ifstream file("tasks.txt");
        if(!file.is_open()){
            cout << "Bilgi: Önceki Kayıt Bulunamadı. Restarting!" << endl;
            return;
        }
        tasks.clear();
        string line;
        int maxid = 0;
        
        while (getline(file, line)){
            size_t pos1 = line.find('|');
            size_t pos2 = line.find('|', pos1 + 1);
            
            if(pos1 != string::npos && pos2 != string::npos){
                task t;
                t.id = stoi(line.substr(0, pos1));
                t.description = line.substr(pos1 + 1, pos2 - pos1 - 1);
                t.isComplated = (line.substr(pos2 + 1) == "1" );
                
                tasks.push_back(t);
                if(t.id > maxid){
                    maxid= t.id;
                }
            }
        }
        nextid = maxid + 1;
        file.close();
        cout << tasks.size() << " Görev Yüklendi" << endl;
    }
    
    void savetofile() const{
        ofstream file("tasks.txt");
        if(!file.is_open()){
            cout << "HATA: Dosya Açılamadı!" << endl;
            return;
        }
        for (const auto& task : tasks){
            file << task.id << "|"
            << task.description << "|"
            << task.isComplated << "\n";
        }
        file.close();
        cout << "Görev Dosyaya Kaydedildi" << endl;
    }
};

void showmenu() {
    cout << "\n====================================" << endl;
    cout << "============To Do Manager============" << endl;
    cout << "=====================================" << endl;
    cout << "1. Görevleri Listele" << endl;
    cout << "2. Yeni Görev Ekle" << endl;
    cout << "3. Görevi Tamamlandı Olarak İşaretle" << endl;
    cout << "4. Görevi Sil" << endl;
    cout << "5. Çıkış(Veriler Henüz Kalıcı Değil)" << endl;
    cout << "Seçiminizi Yapınız: ";
}



    

int main(){
    taskmanager manager;
    manager.loadfromfile();
    int choise;
    string input;
    int taskid;
    
    while(true){
        showmenu();
        if(!(cin >> choise)){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nGeçersiz giriş. Lütfen bir sayı giriniz." << endl;
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch(choise){
            case 1:
                manager.viewtasks();
                break;
            case 2:
                cout << "Yeni Görev Açıklaması Girin:";
                getline(cin, input);
                if(!input.empty()){
                    manager.addtask(input);
                } else {
                    cout << "Görev Açıklaması Boş Bırakılamaz!" << endl;
                }
                break;
            case 3:
                cout << "Tamamlanacak Görev ID'sini Girin:";
                if(cin >> taskid){
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    manager.marktaskcomplated(taskid);
                } else {
                    cout << "HATA: Geçersiz ID Girişi!" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                break;
            case 4:
                cout << "Silinecek Görev ID'sini Girin:";
                if(cin >> taskid){
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    manager.removetask(taskid);
                } else {
                    cout << "HATA: Geçersiz ID Girişi!" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                break;
            case 5:
                manager.savetofile();
                cout << "\nProgram Kapatılıyor" << endl;
                return 0;
            default:
                cout << "\nGeçersiz Seçim" << endl;
        }
    }
    return 0;
    
}
    
    

