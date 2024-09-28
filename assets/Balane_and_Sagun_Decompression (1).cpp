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
class Priority_Queue {
public:
    Priority_Queue(){
        head=NULL;
    }
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
            cout<<head->fq;
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

    void Display() {
        init();
        int bitSize=counter();
        if(bitSize==0){
            return;
        }else{
            cout <<"\n" << endl;
            system("pause");
            root = startQueue();
            cout << setw(10) << "Character" << setw(12) << "Frequency" << setw(12) << "ASCII Value" << setw(20) << "Huffman Code" << endl;
            DisplayHuffmanCode(root, "");
            string trav=retrieve(bitSize);
            generateCharacter(root,trav);
        }

    }

    void init() {
        for (int i = 0; i < 256; i++) {
            frequency[i] = 0;
        }
    }

    int counter() {
        const string SAVE_FILE = "save-compression.compression";
        ifstream in(SAVE_FILE);
         int bitSize;
        if (in.is_open()) {

            in>>bitSize;
            string line;

            int freq;
            while (in>>line>>freq) {
                if(line=="SPACE"){
                    line=" ";
                }else if(line=="NEWLINE"){
                    line="\n";
                }else if(line=="TAB"){
                    line="\t";
                }
                frequency[(int)line[0]]=freq;
                cout <<line<<"\t"<<freq<<endl;
            }
            return bitSize;
        }
    return 0;
    }

    TreeNode *startQueue() {
        for (int i = 0; i < 256; i++) {
            if (frequency[i] > 0) {
                TreeNode *newN = new TreeNode((char)i, frequency[i]);
                Q.enqueue(newN);
            }
        }
        TreeNode *temp=Q.dequeue();
        return temp;
    }

    void DisplayHuffmanCode(TreeNode *r, string huffcode) {
    TreeNode *temp=r;

    if (temp == NULL) {
            return;
        } else {
            DisplayHuffmanCode(temp->left, huffcode + "0");
            DisplayHuffmanCode(temp->right, huffcode + "1");
            if (temp->left == NULL && temp->right == NULL) {
                cout << setw(10) << temp->ch << setw(12) << frequency[(int)temp->ch] << setw(12) << (int)temp->ch << setw(20) << huffcode << endl;
            }
        }
    }

    string retrieve(int sizeBit){
    const string COMPRESSED_FILE = "compressFile.dmcb and mlas";
    ifstream fp(COMPRESSED_FILE);
    string BITS, ORIG;
    if(fp.is_open()){

        char c;
        while(fp.get(c)){
            int ch=(int)c;
            int origCH=ch-32;
            string binary = "";
            if (origCH == 0) {
                binary="000000";
            }else{
                while (origCH > 0) {
                int remainder = origCH % 2;
                binary = static_cast<char>('0' + remainder) + binary;
                origCH /= 2;
                }
                    while (binary.length() < 6) {
                    binary = '0' + binary;
                }
            }

            BITS+=binary;
            binary="";
        }
        int index=0;
        while(index<sizeBit){
            ORIG+=BITS[index];
            index++;
        }
        cout<<ORIG<<endl;
        }
        system("cls");
        return ORIG;
    }

    void generateCharacter(TreeNode *r, string& trav){
    TreeNode *temp=r;
    if(temp!=NULL){
    cout << "\nAfter File Decompression, here's what inside the 'program.txt' file: " << endl;
    cout <<"\n" << endl;
    system("pause");
    system("cls");
        for(char c: trav){
            if(c=='1'){
                temp=temp->right;
            }else if(c=='0'){
                temp=temp->left;
            }
            if(temp->left==NULL && temp->right==NULL){
                cout<<temp->ch;
                temp=r;

            }
        }
        cout << "\n\nDecompression completed!" << endl;
        system ("pause");
    }
    }


private:
    TreeNode *root;
};

int main() {

    Tree hf;

    hf.Display();


return 0;
}
