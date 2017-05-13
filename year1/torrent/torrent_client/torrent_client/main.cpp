#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <sstream>

#include "cpp-bencoding-master/include/Decoder.h"
#include "cpp-bencoding-master/include/Encoder.h"
#include "cpp-bencoding-master/include/PrettyPrinter.h"

#include <iomanip>
#include <stdio.h> /* printf, sprintf */
#include <stdlib.h> /* exit */
#include <unistd.h> /* read, write, close */
#include <string.h> /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h> /* struct hostent, gethostbyname */
#include <fcntl.h> // File control definitions
#include <vector>

using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::stringstream;
using std::ostringstream;
using std::hex;
using std::uppercase;
using std::nouppercase;
using std::vector;

using namespace bencoding;

void error(const char *msg) { perror(msg); exit(0); }

namespace {

/**
* @brief Checks if help was requested.
*/
bool helpIsRequested(int argc, char **argv) {
    if (argc != 2) {
        return false;
    }

    std::string firstArg(argv[1]);
    if (firstArg == "-h" || firstArg == "--help") {
        return true;
    }

    return false;
}

/**
* @brief Prints help to the standard output.
*/
void printHelp(const std::string &prog) {
    std::cout
        << "A decoder of bencoded files.\n"
        << "\n"
        << "Usage: " << prog << " [FILE]\n"
        << "\n"
        << "If FILE is not given, the data are read from the standard input.\n"
        << "The decoded data are printed to the standard output.\n";
}

} // anonymous namespace

string url_encode(const string &value) {
    ostringstream escaped;
    escaped.fill('0');
    escaped << hex;

    for (string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
        string::value_type c = (*i);

        // Keep alphanumeric and other accepted characters intact
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
            continue;
        }

        // Any other characters are percent-encoded
        escaped << uppercase;
        escaped << '%' << std::setw(2) << int((unsigned char) c);
        escaped << nouppercase;
    }

    return escaped.str();
}

vector<string> getTracker(string &data, string req)
{
    vector<string> res;

    int pos1 = 0, pos2, pos3;
    while (pos1 != string::npos && pos2 != string::npos && pos3 != string::npos)
    {
        string subreq = string("\"") + req + string("\"");


        pos1 = data.find(subreq, pos1);



        if (pos1 == string::npos)
            break;
        pos2 = data.find('"', pos1 + req.length()+2);
        pos3 = data.find('"', pos2 + 1);

        if (pos2 != string::npos && pos3 != string::npos)
        {
            res.push_back(data.substr(pos2+1, pos3-pos2-1));

        }

        pos1 = pos3+1;
    }

    return res;
}

vector< vector<int> > getPeers(string &data)
{
    vector <vector <int> > res;

    std::cout << data << std::endl;
    int ndex = data.find("peers");
    if (ndex != string::npos)
    {
        cout << "_________________________ " << ndex << endl;
        ndex = data.find(':', ndex + 1);

        cout << "_________________________ " << ndex << endl;
        for (int i = ndex + 1; i < data.length(); ++i)
        {
            if (i + 6 >= data.length())
                break;

            res.push_back(vector<int>());
            for (int j = i; j <= i + 5; ++j)
            {
                res[res.size()-1].push_back(data[j]);
            }
        }
    }
}

int main(int argc, char **argv)
{
    // Part1 - Parsing file -------------------------------------------------------------

    string filename;
    cout << "Enter filename : ";
    filename = "sample.torrent";

    std::shared_ptr<BItem> decodedData;
    try
    {
        std::ifstream input(filename);
        cout << "File opened, now parsing...";
        decodedData = decode(input);
        cout << "File parsed" << endl;
    }
    catch (const DecodingError &ex)
    {
        std::cerr << "Error: " << ex.what() << endl;
        return 1;
    }

    string repr = getPrettyRepr(decodedData).first;
    cout << repr << endl;

    string infostr = encode(std::shared_ptr<BItem>(getPrettyRepr(decodedData).second));
    vector <string>trackers = getTracker(repr, "announce");

    cout << trackers[0] << endl;
    cout <<  infostr << endl;

    // Part2 - Server request -------------------------------------------------------------

    int sock;
        struct sockaddr_in client;
        int PORT = 6881;

        struct hostent * host = gethostbyname("tracker.tfile.me");



        if ( (host == NULL) || (host->h_addr == NULL) )
        {
            cout << "Error retrieving DNS information." << endl;
            exit(1);
        }

        sock = socket(AF_INET, SOCK_STREAM, 0);

        if (sock < 0) {
            cout << "Error creating socket." << endl;
            exit(1);
        }


        // 6881
        for (PORT = 80; PORT < 6890; ++PORT)
        {
            cout << "trying to connect port " << PORT << "...";
            cout.flush();
            bzero(&client, sizeof(client));
            client.sin_family = AF_INET;
            client.sin_port = htons(PORT);

            memcpy(&client.sin_addr, host->h_addr, host->h_length);

            if (connect(sock, (struct sockaddr *)&client, sizeof(client)) != 0 )
            {
                cout << "failed" << endl;
            }
            else
            {
                cout << "ok!"<<endl;
                break;
            }
        }
        if (PORT == 6890)
        {
            cout << "Couldn't connect" << endl;
            close(sock);
            exit(1);
        }

    // --------------------------------------

    cout<< "still ok!" << endl;

    stringstream ss;
    ss << "GET /announce.php?"
       << //url_encode(
          string("ak=11b9569798&&info_hash=%89M%19x%93%ab%d2%2b%14%5d%8b%a2Z%28%0dT%1e%e8%0d2")+
          "&port=80"+
          "&uploaded=0"+
          "&downloaded=0"+
          "&left=1273509"+
          "&event=started"
       << "HTTP/1.0\r\n"
       << "Host: tracker.tfile.me\r\n"
       << "User-Agent: Transmission v.30000000\r\n"
       << "Accept: text/plain\r\n"
       << "\r\n\r\n";
    string request = ss.str();

    if (send(sock, request.c_str(), request.length(), 0) != (int)request.length()) {
        cout << "Error sending request." << endl;
        exit(1);
    }

    cout << "------------------ ACCEPTING RESPONSE ------------------" << endl;
    char cur;
    string response;
    while ( read(sock, &cur, 1) > 0 ) {
        response += cur;
    }

    cout << response << endl;



    // HAND SHAKING -------------------------------------------------

    vector <vector <int> > peers = getPeers(response);
}
