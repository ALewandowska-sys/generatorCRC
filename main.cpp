#include <iostream>
#include <fstream>
#include <bitset>
#include <sstream>
using namespace std;

string calculateCRC(string keyCRC,string messageBIN){
  string crc=messageBIN;
  string result;
  int keylen = keyCRC.length();
  //we have to add some extra 0
  for(int i = 1; i<= keylen - 1; i++){
    crc += "0";
  }
  int crclen = crc.length();
  int i = 0;

  while(i <= crclen - keylen){
    for(int j=0; j<keylen; j++){
      if(crc[i+j] == keyCRC[j]){  //XOR
        crc[i+j] = '0';
      }
      else{
        crc[i+j] = '1';
      }
    }
    if(i<crclen && crc[i]!='1') i++;
    }
  result = crc.substr(crclen - keylen + 1);

  if(result.length() % 8 != 0){
    int howMany = 8-result.length();
    for(int i = 1; i <= howMany; i++) {
      result += "0";
    }
  }

  return result;
}


void checkCRC(string keyCRC,string messageBIN){
  string crc = messageBIN;
  int keylen = keyCRC.length();
  int crclen = crc.length();
  int i = 0, result;

  while(i <= crclen - keylen){
    for(int j=0; j<keylen; j++){
      if(crc[i+j] == keyCRC[j]){  //XOR
        crc[i+j] = '0';
      }
      else{
        crc[i+j] = '1';
      }
    }
    if(i<crclen && crc[i]!='1') i++;
  }

  //we dont know how many 0 we have, so we convert string to int
  istringstream ( crc.substr(crclen - keylen) ) >> result;

  if(result == 0){
    cout << "CRC is correct!\n";
  }
  else{
    cout << "CRC is incorrect\n";
  }
}



int main(){
  int choice;

  cout<<"1. Calculate\n2. Check\n3. Exit\n";
  cin >> choice;

  while(choice!=3)
  {
    string message ="", keyCRC, messageBIN ="",fileName, result;
    cout<<"Enter the filename: ";
    cin >> fileName;
    fstream MyReadFile(fileName);
    while(!MyReadFile){
        cout<<"Error, something it is wrong  with file"<<endl;
        cout<<"Enter the filename: ";
        cin >> fileName;
        fstream MyReadFile(fileName);
    }
    cout <<"Enter the key: ";
    cin >> keyCRC;

    cout<<"\nInput file in ASCII: "<< endl;

    //take every character from file
    message.assign( (istreambuf_iterator<char>(MyReadFile) ),(istreambuf_iterator<char>()) );

    cout<< message;
    cout<<"\nInput file in binary: "<< endl;

    for (size_t i = 0; i < message.size(); ++i)
     {
       bitset<8> b(message.c_str()[i]);
       messageBIN += b.to_string();
     }
    cout<< messageBIN;


    switch(choice)
    {
      case 1:
      {
        result = calculateCRC(keyCRC, messageBIN);
        cout <<"\n\nRemainder: " + result << endl;

        //we must convert int to ascii and write at the end of file
        stringstream sstream(result);
        while(sstream.good())
        {
            bitset<8> bits;
            sstream >> bits;
            char c = char(bits.to_ulong());
            MyReadFile << c;
        }
        break;
      }
      case 2:
      {
        cout << "\n\nChecking CRC...";
        checkCRC(keyCRC, messageBIN);
        break;
      }
      case 3:
        exit(0);
      default:
        {
          cout<<"I dont understand";
          exit(0);
        }
    }
    cout << endl;
    MyReadFile.close();

    cout<<"1. Calculate\n2. Check\n3. Exit\n";
    cin >> choice;

  }

  return 0;
}

