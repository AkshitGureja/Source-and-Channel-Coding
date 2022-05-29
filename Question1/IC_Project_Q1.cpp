#include <bits/stdc++.h>
using namespace std;

// typedef a pointer to struct Node as ptr 
typedef struct Node* ptr;

// typedef struct Node as node to make it easier to refer in future
typedef struct Node Node;

// The struct node has the character and the frequency corresponding to it.
struct Node
{
    int freq;
    string ch; // or char only
    ptr left;
    ptr right;
};

// custom comparator function to sort the struct according to the frequency of characters
bool cmp(ptr A, ptr B)
{
    if(A->freq > B->freq){
        return true;
    }
    else{
        return false;
    }
}

// function to initialise a node and get memory for it
ptr init_node(int freq, string ch)
{
    ptr node = new Node;
    node->freq = freq;
    node->ch = ch;
    node->left = NULL;
    node->right = NULL;

    return node;
};

// reference to the PrintCodewords function with code below after main()
void PrintCodewords(ptr root, string str);

// initalises a vector of pair of strings to store the character and the codeword corresponding to it
vector<pair<string, string>> codewords;

int main()
{
    // declares a vector of pointer to struct Node and the vector is called N
    vector<ptr> N;
    char c;
    FILE *fp;

    // opens the file with text to be encoded
    fp = fopen("file_3.txt", "r");

    while(1)
    {
        // scans the opened file character by character till the time we get EOF i.e. End Of File. 
        // When we reach EOF, we break the loop
        if(fscanf(fp, "%c", &c) == EOF){
            break;
        }
        else{
            // flag to check if the character has been seen for the first time
            int init_flag = 0;
            for(int j = 0; j < N.size(); j++){
                if(N[j]->ch == string(1, c)){
                    // if the character already has been seen once and we have found it in N, we put init_flag = 1 and increase its frequency by 1
                    init_flag = 1;
                    N[j]->freq++;
                    break;
                }
            }
            if(init_flag == 0){
                // if the character is not yet in the vector N, we have init_flag = 0 and we declare a node corresponding to it with frequency 1
                ptr D = init_node(1,string(1,c));

                // this new node corresponding to the new character which we came across for the first time is then pushed into the vector N
                N.push_back(D);
            }
        }
    }
    fclose(fp); // closes the file as it is no longer required

    // loop to iterate over N, and prints the characters and their frequencies
    cout << "Characters and their frequencies" << endl;

    for(int i = 0; i < N.size(); i++){
        if(N[i]->ch == "\n"){
            cout << "next line" << " : " << N[i]->freq << endl;
        }
        else{
            cout << N[i]->ch << " : " << N[i]->freq << endl;
        }
    }
    cout << endl;

    // TO construct the Huffman Tree according to the algorithm
    // when the size of the array becomes 1, it will be the root of the tree formed
    while(N.size() != 1)
    {
        // sort the vector N from beginning to end according to the frequencies which is done by our custom comparator function cmp
        sort(N.begin(), N.end(), cmp);

        int size = N.size();

        // we create 2 new nodes to duplicate the characters with the least frequency, and get a pointer to them

        // E is a pointer to the replica of node with least frequency
        ptr E = init_node(N[size-1]->freq, N[size-1]->ch);

        // F is a pointer to the replica of node with 2nd least frequency
        ptr F = init_node(N[size-2]->freq, N[size-2]->ch);

        // we initialise a new node with pointer root for joining the 2 nodes with least frequency. 
        // This node has the frequency equal to sum of the 2 frequencies of its children
        ptr root = init_node(E->freq+F->freq, E->ch + F->ch);

        // making the left pointer of root point to E and right to F
        root->right = E;
        root->left = F;

        // Now children of the tree formed till now must also be a part of this new tree
        E->left = N[size-1]->left;
        E->right = N[size-1]->right;
        F->left = N[size-2]->left;
        F->right = N[size-2]->right;

        // we remove the least frequency node from the vector
        N.pop_back();

        // we remove the 2nd least frequency node also from the vector
        N.pop_back();

        // we push into vector the new node which has the frequency as sum of least 2
        N.push_back(root);
    }

    // initialize a string, with the help of which we will print codewords
    string str = ""; 

    cout << "Printing the codewords" << endl;

    // calling the PrintCodewords function
    PrintCodewords(N[0], str);

    // We now have the codewords corresponding to each character we enocunter in the file to be encoded

    // ENCODING
    // to encode we open the file again, and character by character we will encode it into bits using table lookup. 
    // The table here is our codewords array which we defined globally in the beginning


    // we open the file to be encoded
    FILE * fp1;
    fp1 = fopen("file_3.txt", "r");
    
    // using ofstream we open the file to which we have to write
    // We will write to a file names encoded_file.txt which doesn't exist till now. 
    // So ofstream will create a txt file and write to it
    ofstream MyFile("encoded_file.txt");

    // we will encode the characters into codewords till the time we don't encounter the End Of File or the EOF
    while(1){
        // when we reach the EOF, we break the loop
        if(fscanf(fp1, "%c", &c) == EOF){
            break;
        }
        else{
            // loop used for table lookup, to get the codeword corresponding to each character we wish to encode
            for(int k = 0; k < codewords.size(); k++){
                // when we find the character, we print the codeword corresponding to that
                if(codewords[k].first == string(1,c)){
                    MyFile << codewords[k].second;
                }
            }
        }
    }
    // we close both the file to be encoded and the encoded file
    fclose(fp);
    MyFile.close();

    // DECODING
    
    FILE *fp2;

    // opens the encoded txt file
    fp2 = fopen("encoded_file.txt", "r");

    // opens a new file named decoded.txt to which we will write the decoded text
    ofstream MyFile_2("decoded_file.txt");

    // we get a pointer to the root
    ptr t = N[0];

    // we now traverse the tree whenever according to the bit we receive
    while(1){   
        // we scan the encoded binary file till the time we don't reach the EOF
        if(fscanf(fp2, "%c", &c) == EOF){
            break;
        }
        else{   
            // if we get a '0', we move to the left of the tree
            if(c == '0'){
                t = t->left;
            }
            else{
                // if we get the bit '1', we move on the right branch of the tree
                t = t->right;
            }
            // as soon as we come across a node which is a leaf node, has no successors, we know that it will valid character and we output that character
            if(t->left == NULL)  {
                MyFile_2 << t->ch;
                t = N[0];
            }
        }
    }

    return 0;
}

// Recursive function PrintCodewords
void PrintCodewords(ptr root, string str)
{
    // base case, if root is NULL, there is nothing to traverse and we break
    if(root == NULL){
        return;
    }
    else{
        // print all codewords to the left. We add a 0 to the codeword 
        PrintCodewords(root->left, str + "0");

        // Print all codewords to the right. We add a 1 to the codeword
        PrintCodewords(root->right, str + "1");

        // if it is a leaf node, so the pointers LEFT and RIGHT will point to NULL.
        // This indicates that we need to print the codeword at this point because we are now at a leaf node and will have a character which was encoded
        if(root->left == NULL){

            //cout << root->ch << " " << str << endl;
            if(root->ch == "\n"){
            cout << "next line" << " : " << str << endl;
            }
            else{
            cout << root->ch << " " << str << endl;
            }

            // we add this codewords node and character to our table which is used while decoding.
            codewords.push_back(make_pair(root->ch, str));
        }     
    }
}