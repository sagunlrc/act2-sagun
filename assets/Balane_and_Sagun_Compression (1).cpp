#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

struct TreeNode {
    int fq;
    char ch;
    TreeNode *left, *right, *next;

    TreeNode(char c, int x) : ch(c), fq(x), left(NULL), right(NULL), next(NULL) {}

    TreeNode(int x, TreeNode *l, TreeNode *r) : ch('\0'), fq(x), left(l), right(r), next(NULL) {}
};

string filename;


class Priority_Queue {
public:
    void enqueue(TreeNode *newNode) {
    if (!head || newNode->fq < head->fq) {
        newNode->next = head;
        head = newNode;
    } else {
        TreeNode *current = head;
        TreeNode *previous = NULL;

        while (current != NULL && newNode->fq >= current->fq) {
            previous = current;
            current = current->next;
        }

        newNode->next = current;

        if (previous) {
            previous->next = newNode;
        } else {
            head = newNode;
        }
    }
}
    TreeNode *dequeue() {
        if (head == NULL) {
            cout << "Queue is empty." << endl;
            return NULL;
        } else {
            while (head->next != NULL) {
                TreeNode *left = head;
                TreeNode *right = head->next;
                int sum = left->fq + right->fq;
                TreeNode *rootNode = new TreeNode(sum, left, right);
                enqueue(rootNode);
                head = head->next->next;
            }
            TreeNode *temp = head;
            head = NULL;
            return temp;
        }
    }

private:
    TreeNode *head;
};

class Tree {
    int frequency[256];
    string huffmancode[256];
    Priority_Queue Q;

public:
    Tree() {
        root = NULL;
    }

    void Display(const string file) {
        init();
        counter(file);
        root = startQueue();
        system("cls");
        cout << setw(40) << "Character" << setw(12) << "Frequency" << setw(12) << "ASCII Value" << setw(20) << "Huffman Code" << endl;
        cout<<endl;
        DisplayHuffmanCode(root, "");
        createBit(file);
    }

    void init() {
        for (int i = 0; i < 256; i++) {
            frequency[i] = 0;
        }
    }

    void counter(const string file) {
        ifstream in(file);
        if (in.is_open()) {
            char c;
            while (in.get(c)) {
                cout << c << endl;
                frequency[(int)c]++;
            }
        }
    }

    TreeNode *startQueue() {
        for (int i = 0; i < 256; i++) {
            if (frequency[i] > 0) {
                TreeNode *newNode = new TreeNode((char)i, frequency[i]);
                Q.enqueue(newNode);
            }
        }
        return Q.dequeue();
    }

    void DisplayHuffmanCode(TreeNode *r, string huffcode) {
        if (r == NULL) {
            return;
        } else {
            DisplayHuffmanCode(r->left, huffcode + "0");
            DisplayHuffmanCode(r->right, huffcode + "1");
            if (r->left == NULL && r->right == NULL) {
                huffmancode[(int)r->ch] = huffcode;
               cout << setw(35) << r->ch << setw(12) << frequency[(int)r->ch] << setw(12) << (int)r->ch << setw(20) << huffcode << endl;
            }
        }
    }

    void createBit(const string& file) {
    ifstream in(file);
    if (in.is_open()) {
        char c;
        string bits;
        while (in.get(c)) {
            bits += huffmancode[(int)c];
        }

        save(bits);
        int bitSize = bits.length();
        int remainder = bitSize % 6;

        if (remainder > 0) {
            for (int i = remainder; i < 6; i++) {
                bits = bits + "1";
            }
        }
        Bits_to_ASCII(bits);
    } else {
        cerr << "Error opening file." << endl;
    }
}

void Bits_to_ASCII(const string& bits) {
    int bitSize = bits.length();
    int index = 0;
    int count = 0;
    string padded, character;
    while (index < bitSize) {

        count++;
        padded += bits[index];
        if (count == 6) {
            int ascii = stoi(padded, NULL, 2);
            int add = ascii + 32;
            character += static_cast<char>(add);
            count = 0;
            padded = "";
        }
        index++;
    }
    cout << "\n\t\t\t\t-----------------------------------------------" << endl;
    cout <<"\t\t\t\tcompress completed!" << endl;
    cout <<"\t\t\t\tcompression saved to 'compressFile.DMCB and MLAS'." << endl;
    cout << "\t\t\t\t-----------------------------------------------" << endl;

    ofstream out("compressFile.dmcb and mlas");
    if (out.is_open()) {
        out << character;
    }
}

    void save(string bits) {
        ofstream out("save-compression.compression");
        if (out) {
            out << bits.length() << endl;
            for (int i = 0; i < 256; i++) {
                if (frequency[i] > 0) {
                    if ((char)i == 10)
                        out << "NEWLINE" << "\t" << frequency[i] << endl;
                    else if ((char)i == 32)
                        out << "SPACE" << "\t" << frequency[i] << endl;
                    else if ((char)i == 9)
                        out << "TAB" << "\t" << frequency[i] << endl;
                    else
                        out << (char)i << "\t" << frequency[i] << endl;
                }
            }
        }
    }

private:
    TreeNode *root;
};

int main() {
    Tree hf;
    cout <<"\n\n\n\n\n\n\n\n\t\t\t\t\t\t\tENTER FILENAME: ";

    cin>>filename;
    const string file=filename+".txt";
    ifstream read(file);
    if (!read.is_open()) {
       cout  << "\t\t\t\t\t\t\t----------------------------" << endl;
        cerr  << "\t\t\t\t\t\t\tError opening file." << endl;
        cout  << "\t\t\t\t\t\t\tInvalid Filename." << endl;
        cout  << "\t\t\t\t\t\t\tPlease try again." << endl;
        cout  << "\t\t\t\t\t\t\t----------------------------" << endl;
        cout  << "\t\t\t\t\t\t\t";
        system("pause");
        system("cls");
        return main();
    } else {
        hf.Display(file);
    }
    return 0;
}
