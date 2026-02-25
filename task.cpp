#include <iostream>
#include <vector>
#include <unordered_map>
#include <string.h>
#include <cstring>
#include <fstream>

struct ListNode {
    ListNode* prev = nullptr;
    ListNode* next = nullptr;
    ListNode* rand = nullptr;
    std::string data;
};

class Serializer
{
    public:
        ListNode* readfile(const std::string&);
        void serialize(ListNode*, const std::string&);
        ListNode* deserialize(const std::string&);
        void freeList(ListNode*);
        void printList(ListNode*);

};
ListNode* Serializer :: readfile(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Не удалось открыть файл: " << filename << std::endl;
        return nullptr;
    }
    std::vector<ListNode*> nodes;
    std::vector<int> index;
    std::string str;
    while (std::getline(file,str))
    {
        if (str.empty()) continue;
        size_t pos = str.rfind(';');
        if (pos == std::string::npos) continue;
        std::string data = str.substr(0, pos); //  apple;3 - > apple
        int randindex = std::stoi(str.substr(pos + 1)); // apple;3 - > 3
        ListNode* node = new ListNode();
        node->data = data;
        nodes.push_back(node);
        index.push_back(randindex);

    }
    file.close();
    if (nodes.empty()) {
        return nullptr;
    }
    // Установка prev и next указателей
    for (size_t i = 0; i < nodes.size(); ++i) {
        if (i > 0) {
            nodes[i]->prev = nodes[i - 1];
        }
        if (i < nodes.size() - 1) {
            nodes[i]->next = nodes[i + 1];
        }
    }
    for (size_t i = 0; i < nodes.size(); ++i) {
        if (index[i] >= 0 && index[i] < static_cast<int>(nodes.size())) {
            nodes[i]->rand = nodes[index[i]];
        }
    }

    return nodes[0];
}
void Serializer :: serialize(ListNode* head, const std::string& filename)
{
    if (head == nullptr)
    {
        return;
    }
    std::ofstream file(filename);
    if (!file.is_open()) {
       std::cerr << "Не удалось открыть файл для записи: " << filename << std::endl;
       return;
    }
    std::unordered_map<ListNode*, int> nodeandIndex;
    std::vector<ListNode*> nodes;
   
    ListNode* current = head;
    int index = 0;
    while (current) {
        nodeandIndex[current] = index++;
        nodes.push_back(current);
        current = current->next;
    }
    
    // Запись количества узлов
    file << nodes.size() << "\n";

    for (ListNode* node : nodes) {
        // Запись индекса rand (-1 если nullptr)
        int randIndex = (node->rand) ? nodeandIndex[node->rand] : -1;
        file << randIndex << "\n";
        
        // Запись длины строки data
        file << node->data.length() << "\n";
        
        // Запись самой строки data
        file << node->data << "\n";
    }

    file.close();
}
ListNode* Serializer :: deserialize(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Не удалось открыть файл для чтения: " << filename << std::endl;
            return nullptr;
        }

        // Чтение количества узлов
        int nodeCount;
        file >> nodeCount;
        file.ignore(); // пропуск символа новой строки

        if (nodeCount <= 0) {
            file.close();
            return nullptr;
        }

        std::vector<ListNode*> nodes(nodeCount);
        std::vector<int> randIndices(nodeCount);

        // Чтение всех узлов
        for (int i = 0; i < nodeCount; ++i) {
            nodes[i] = new ListNode();

            // Чтение индекса rand
            file >> randIndices[i];
            file.ignore();

            // Чтение длины строки data
            int dataLength;
            file >> dataLength;
            file.ignore();

            // Чтение строки data
            std::getline(file, nodes[i]->data);
        }

        file.close();

        // Установка prev и next указателей
        for (int i = 0; i < nodeCount; ++i) {
            if (i > 0) {
                nodes[i]->prev = nodes[i - 1];
            }
            if (i < nodeCount - 1) {
                nodes[i]->next = nodes[i + 1];
            }
        }

        // Установка rand указателей
        for (int i = 0; i < nodeCount; ++i) {
            if (randIndices[i] >= 0 && randIndices[i] < nodeCount) {
                nodes[i]->rand = nodes[randIndices[i]];
            }
        }

        return nodes[0];
}
void Serializer :: printList(ListNode* head) {
        std::unordered_map<ListNode*, int> nodeToIndex;
        ListNode* current = head;
        int index = 0;
        
        while (current) {
            nodeToIndex[current] = index++;
            current = current->next;
        }

        current = head;
        index = 0;
        while (current) {
            int randIndex = (current->rand) ? nodeToIndex[current->rand] : -1;
            std::cout << "Узел " << index << ": \"" << current->data 
                      << "\" -> rand на узел " << randIndex << std::endl;
            current = current->next;
            index++;
        }
    }
void Serializer :: freeList(ListNode* head) {
    while (head) {
        ListNode* next = head->next;
        delete head;
        head = next;
    }
}
int main() {
    Serializer serializer;

    // Чтение из текстового файла
    ListNode* head = serializer.readfile("intet.in");
    
    if (!head) {
        std::cerr << "Ошибка чтения списка" << std::endl;
        return 1;
    }

    std::cout << "\nИсходный список:" << std::endl;
    serializer.printList(head);


    std::cout << "\nСериализация в outlet.out" << std::endl;
    serializer.serialize(head, "outlet.out");

    std::cout << "Десериализация из outlet.out" << std::endl;
    ListNode* deserializedHead = serializer.deserialize("outlet.out");

    if (deserializedHead) {
        std::cout << "\nДесериализованный список:" << std::endl;
        serializer.printList(deserializedHead);
    }


    serializer.freeList(head);
    serializer.freeList(deserializedHead);

    return 0;
}
