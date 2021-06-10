#include <iostream>
#include <map>
#include <iterator>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
 
using namespace std;

const string symbols = ".,/?:;#@!$%^&*()_{}<>'[]0123456789–";
const char theOne = '"';
const string separation = "-_+–";

void removeFrontSymbols(string &word)
{
   for(char c : symbols)
   {
        if(word.front() == c) 
        {
            word = word.substr(1, word.size());
            removeFrontSymbols(word);
        }
        if(word.front() == theOne)
        {
           word.substr(1, word.size());
           removeFrontSymbols(word);
        }
   }
}

void removeBackSymbols(string &word)
{
   for(char c : symbols)
   {
      if(word.back() == c)
      {
         word.pop_back();
         removeBackSymbols(word);
      }
      if(word.back() == theOne)
      {
         word.pop_back();
         removeBackSymbols(word);
      }
   }
}

vector<string> separateWords (string separatableWord)
{
   vector<string> words;
   for(auto separator : separation)
   {
      auto pos = separatableWord.find(separator);
      if(pos != string::npos)
      {
         words.push_back(separatableWord.substr(0, pos));
         words.push_back(separatableWord.substr(pos+1, separatableWord.size()));
         break;
      }
   }
   return words;
}

bool doesWordHaveDot(const string &word)
{
   for(auto letter : word)
   {
      if(letter == '.') return true;
   }
   return false;
}

bool doesWordHaveSeparation(string &word)
{
   for(auto separator : separation)
   {
      if(word.find(separator) != string::npos) return true;
   }
   return false;
}

bool isWordLink(const string &word)
{
   if(doesWordHaveDot(word)) return true;
   else if(word.size() > 4 && word.substr(0, 4) == "www.") return true;
   else if(word.size() > 8 && word.substr(0, 8) == "https://") return true;
   else return false;
}

struct word{
   int count = 1;
   vector<int> wordIndicated;
};
 
int main()
{
   string fileName = "text.txt";
   int lineNumber = 1;
   map<string, word> words;
   vector<string> links;

   fstream file;
   file.open(fileName);

   stringstream fileStream;
   fileStream << file.rdbuf();

   file.close();

   ofstream fr ("results.txt");

   while(!fileStream.eof())
   {
      string tempLine;
      string oneWord;
      getline(fileStream, tempLine);
      stringstream lineStream(tempLine);

      while(!lineStream.eof())
      {
         lineStream >> oneWord;
         removeFrontSymbols(oneWord);
         removeBackSymbols(oneWord);

         if(oneWord.size() != 0)
         {
            auto temp = words.find(oneWord);
            if(isWordLink(oneWord)) links.push_back(oneWord);
            else if(doesWordHaveSeparation(oneWord))
            {
               vector<string> separateWord = separateWords(oneWord);
               for(auto word : separateWord)
                  lineStream << word;
            } 
            else if(temp == words.end() || words.size() == 0)
            {
               word newWord;
               newWord.wordIndicated.push_back(lineNumber);
               words.insert(make_pair(oneWord, newWord));
            }
            else
            {
               temp -> second.count++;
               temp -> second.wordIndicated.push_back(lineNumber);
            }
         }
      }
      lineNumber++;
      
   }

   for(auto word : words)
   {
      if(word.second.count > 1)
      {
         fr << word.first << setw(16 - word.first.length() + 9) << " | times=" << setw(2) << word.second.count << "| locations: ";
         for(auto location : word.second.wordIndicated)
         fr << location << " ";
         fr << endl;
      }
   }

   fr << endl;
   fr << "Links: " << endl;
   for(auto link : links)
      fr << link << endl;
 
}