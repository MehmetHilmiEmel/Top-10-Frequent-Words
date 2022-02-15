#include <iostream>  
#include <fstream> //fstream library provides functions for files
#include <string>
#include <ctime>  //because of time
#include <chrono> //chrono library is used to deal with date and time
#include <cmath>  //because of pow function
#include <cstring>
#include <algorithm> //bacuse of remove function

using namespace std;

struct Table
{
    string word;  
    int counter;
    Table()  //constructor of table struct
    {
        word = "";
        counter = 0;
    }
};

int charToInteger(string charNumber)  //transforming char to integer function
{
    int result = 0;  
    for (int i = 0; i < charNumber.size(); i++)
    {
        int a = (int)charNumber[i] - 48;  //We subtract 48 to convert the string into a number 
        result += pow(10, charNumber.size() - i - 1) * a;  //We created a function that converts 1 or more digit numbers to an integer and assigned result variable
    }
    return result;  //Then return the result
}

void insert(Table *hashTable, string insertdata, string charNumber, int hashIndex) //With this function we add the words and their numbers to the hash table
{

    int number = charToInteger(charNumber);  //we convert the letter into an integer with charToInteger function
    while (hashTable[hashIndex].counter != 0 && hashTable[hashIndex].word != insertdata)  //we checked the case where the index 
    {                                                                                     //is filled with a different word
        hashIndex = (hashIndex + 1) % 3000000;  //then we updated the index with the next empty index
    }                                           //we used mode operator with 3000000 because its max size of an array

    if (hashTable[hashIndex].counter == 0)  //If counter is 0, we have added both the word and counter to the hash table. 
    {
        hashTable[hashIndex].word = insertdata;
        hashTable[hashIndex].counter = number;
    }
    else if (hashTable[hashIndex].word == insertdata) //If the word was already in the hash, we just updated its counter.
    {
        hashTable[hashIndex].counter += number;  
    }
}

bool stopFunction(int hash, string word, string *stopwords)  //In stop function we checked if the word is a stopword or not
{

    bool found = false;
    if (stopwords[hash] == word)
    {
        found = true;  //if it is stopword, then found true
                       //otherwise it is false, and we already
                       //defined it as false before
    }

    return found; 
}

int hashFunction(string word) //The hash function we created to find the index
{
    int number = 0; 
    for (int i = 0; i < word.size(); i++)
    {
        number += word[i] * word[i] * (i+1); //We created a function that squares 
                                             //the ASCII value of the word and 
                                             //multiplies it by (i+1).
  
    }
    return number;
}

void makeStopHashTable(string filename, string *stopwords, int StopMax)  //Here we have created a hash table for stopwords
{                                                                        //And we used the same function as hash table to compare words
    fstream data(filename, ios::in);  
    string word;
    int index = 0;
    if (data.is_open())  //Returns whether the stream is currently associated to a fill
    {
        while (data >> word)  // extracting words from the data
        {
            index = hashFunction(word);  
            index = index % StopMax;  //index assigning
            stopwords[index] = word;  //stopword's hashtable term assigning
            index = 0;
        }
    }
    else{
        cout << "File can not be accessible" << endl;  //File can not open
    }
}

void frequentWords(Table *hashTable) //We created this function to print the 10 frequent words on the screen, 
{                                    //which is the purpose of the project.

    for (int i = 0; i < 10; i++)
    {
        int topIndex = 0;
        int topinstance = hashTable[0].counter; 

        for (int j = 0; j < 3000000; j++)  //3000000 is our max size
        {
            if (hashTable[j].counter > topinstance)  //If there is a word with a higher frequency than the index we are in
            {
                topinstance = hashTable[j].counter;  //both counter 
                topIndex = j; //and index are updated
            }
        }

        cout << i + 1 << ".WORD:  " << hashTable[topIndex].word << "-->" << topinstance << endl;  
        hashTable[topIndex].counter = 0; //then we reset the counter and continued
    }
}

int linearSearch(string sentence, string word)  //We created this function to access the word unigramCount, 
{                                               
    int s = sentence.size(); //we got the size of the sentence 
    int w = word.size();  //and size of the word 
    for (int i = 0; i < s - w - 1; i++)  
    {
        int j = 0;
        while (j < w && word[j] == sentence[i + j])  //if a letter is found, the next letter is checked 
            j = j + 1;
        if (j == w) //If the word found, then it returns last index of word 
            return i + word.size();
    }
    return -1; //If the word is not found, the function is terminated with -1.
}

void makeHashTable(string text, Table *hashTable, string *stopwords, int maxSize) //Here, we have created a hash table
{

    string key = "";  //We created a key variable, because this is our word
    string number = "";  //And number variable, we kept the number of repetitions of the word in this variable
    int length = 0;  //and firstly length is 0

    text.erase(remove(text.begin(), text.end(), '-'), text.end());  //By deleting '-' and '}' marks, we were able to select words more easily
    text.erase(remove(text.begin(), text.end(), '}'), text.end());

    length = linearSearch(text, "unigramCount") + 3;  //By adding 3 to this value, we have reached the part where the words begin

    text += ",";

    while (length < text.size())  //If we have reached the end of the line, we have exited the loop
    {
        while (text[length] != ',')  //If we're still in the key, number pairs
        {
            length++;  //We wanted to pass the first " sign and reach the first letter.
            while (text[length] != '"')  //If we're still in the word
            {

                if (text[length] == 92) //If we have '\'
                {
                    length++;  //then we want to pass the index, so that we updated the length
                }
                else if ((text[length] >= 97 && text[length] <= 122) || (text[length] <= 90 && text[length] >= 65)) //if the letter is lower case or upper case 
                {
                    if (text[length] <= 'Z' && text[length] >= 'A')  //if it is upper case then we added 32 to convert it to lower case 
                    {
                        text[length] += 32;
                    }

                    key += text[length]; //then we added these letters into the key variable
                }

                length++;  //and length updated
            }
            if (key == "") //If the key contains no letters (could be a number),
            {
                length--;  //then we decreased the length
                break; //and we and we're out of the loop
            }          //and we move on to the next part

            length++;  
            if (text[length] == ':')  //If we reached ':' sign
            {
                length++;  //We update the length and come to the number part.
 
                while (text[length] != ',')  //If we're still in the key, number pairs
                {
                    number += text[length]; //then we added the number to the number variable
                    length++; //and updated length 
                }             //and the program moves to the next index
                if (!stopFunction(hashFunction(key) % maxSize, key, stopwords))  //If the word we want to add is not stopword
                {
                    insert(hashTable, key, number, hashFunction(key) % maxSize); //The program adds it to the hash table
                }
                number = "";  //we reset our variables for the next word
                key = "";  
            }
        }
        length++;  //and we go to the next word
    }
}

int main()
{
    clock_t tstart = clock();   //started a clock

    int maxSize = 3000000;
    Table *hashTable = new Table[maxSize];  //created hash table 
    string *stopwords = new string[maxSize];  //and stopwords

    makeStopHashTable("stopwords.txt", stopwords, maxSize);  //created hash table for stopwords

    fstream reading;  //text file created
    reading.open("PublicationsDataSet.txt", ios::in);  //text file opened

    string text;
    cout << "File is reading now..." << endl;
    while (reading.good())  //this function will check if this stream has raised any error or not
    {                       //if it is not, then continue
        getline(reading, text);  //reading a line from text
        makeHashTable(text, hashTable, stopwords, maxSize);  //The line read is added to the hash table
    }                                                       

    reading.close();  //text file closed

    frequentWords(hashTable); //After the file is closed, 
                              //frequently used words are printed on the screen
    cout << endl;

    cout << "Time passed:" << (double)(clock() - tstart) / CLOCKS_PER_SEC  //Then we wrote the elapsed time on the screen
         << "sec" << endl;

    return 0;
}
