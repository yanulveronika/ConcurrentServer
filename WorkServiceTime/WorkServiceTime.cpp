#include <stdio.h>
#include <tchar.h>
#include "iostream"
#include "Windows.h"                
#include "ctime"
#include <string>

using namespace std;

bool validTime( string str );
bool correctTimeFromTo( string from, string to );
int* getHoursAndMinutes( string str );


int main( ) {
  setlocale( LC_ALL, "Rus" );
  SetConsoleTitle( "WorkServiceTime" ); // заголовок консольного окна

  try {
    HANDLE mail;
    char bufwrite[ 50 ] = "", bufread[ 50 ] = {};
    DWORD dw;
    int count = 0, i = 0;
    char serverName[ 256 ];
    char MailName[ 512 ];
    char WriteBuf[ 50 ] = "test";
    bool result;
    DWORD nBytesWrite;
    string servname;
    string str;
    string rand_time_from = "07:00";
    string rand_time_to = "08:10";
    string echo_time_from = "07:30";
    string echo_time_to = "08:35";
    string time_time_from = "10:00";
    string time_time_to = "11:30";

    cout << "Enter rand from: ";
    cin >> rand_time_from;

    cout << "Enter rand to: ";
    cin >> rand_time_to;

    cout << "Enter echo from: ";
    cin >> echo_time_from;

    cout << "Enter echo to: ";
    cin >> echo_time_to;

    cout << "Enter time from: ";
    cin >> time_time_from;

    cout << "Enter time to: ";
    cin >> time_time_to;

    cout << "Enter servaname: ";
    cin >> str;

    servname.append( "\\\\" );
    servname.append( str );
    servname.append( "\\mailslot\\box" );

    mail = CreateFile( servname.c_str( ), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL );

    if ( correctTimeFromTo( rand_time_from, rand_time_to ) &&
         correctTimeFromTo( echo_time_from, echo_time_to ) &&
         correctTimeFromTo( time_time_from, time_time_to ) ) {
      string msg;
      msg += rand_time_from + '\n' + rand_time_to + '\n';
      msg += echo_time_from + '\n' + echo_time_to + '\n';
      msg += time_time_from + '\n' + time_time_to + '\n';

      if ( !WriteFile( mail, msg.c_str( ), strlen( msg.c_str( ) ) + 1, &nBytesWrite, NULL ) )
        throw "WriteFile: Ошибка ";

    }
    else {
      cout << "invalid time input" << endl;
    }

    CloseHandle( mail );
  }
  catch ( ... ) {
    cout << "ошибка" << endl;
  }

  system( "pause" );
  return 0;
}

bool validTime( string str ) {
  bool res = false;

  if ( str.length( ) == 5 ) {
  int* arr = getHoursAndMinutes( str );
    if ( arr != NULL ) {
      if ( arr[0] >= 0 && arr[ 0 ] <= 23 && arr[ 1 ] >= 0 && arr[ 1 ] <= 59 ) {
        return true;
      }
    }
  }
  return res;
}

bool correctTimeFromTo( string from, string to ) {
    return true;
}


int* getHoursAndMinutes( string str ) {
  string str_1, str_2;
  if ( str[ 2 ] == ':' ) {
    str_1 += str.substr( 0, 2 );
    str_2 += str.substr( 3, 2 );
    int ihh = atoi( str_1.c_str( ) );
    int imm = atoi( str_2.c_str( ) );
    return new int[ 2 ] { ihh, imm };
  }
  return NULL;
}

