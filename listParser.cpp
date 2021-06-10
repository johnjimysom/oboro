#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <string>
#include <algorithm>
#include <boost/algorithm/string.hpp>
/*
 * A class to read data from a csv file.
 */
class CSVReader
{
    std::string fileName;
    std::string delimeter;
public:
    CSVReader(std::string filename, std::string delm = ",") :
        fileName(filename), delimeter(delm)
    { }
    // Function to fetch data from a CSV File
    std::vector<std::vector<std::string> > getData();
};
/*
* Parses through csv file line by line and returns the data
* in vector of vector of strings.
*/
std::vector<std::vector<std::string> > CSVReader::getData()
{
    std::ifstream file(fileName);
    std::vector<std::vector<std::string> > dataList;
    std::string line = "";
    // Iterate through each line and split the content using delimeter
    while (getline(file, line))
    {
        std::vector<std::string> vec;
        boost::algorithm::split(vec, line, boost::is_any_of(delimeter));
        dataList.push_back(vec);
    }
    // Close the File
    file.close();
    return dataList;
}


int main()
{
    // Creating an object of CSVWriter
    CSVReader reader("qr-scan-sample.csv");
    // Get the data from CSV File
    std::vector<std::vector<std::string> > dataList = reader.getData();
    // Print the content of row by row on screen
    for (std::vector<std::string> vec : dataList)
    {
        for (std::string data : vec)
        {
            std::cout << data << " , ";
        }
        std::cout << std::endl;
    }
    return 0;

    //int main() { parseQR::main(); }//misplaced correctly
}

//creating two main functions?
/*namespace parseQR {
    int main() { return 0; }
}

int main() { parseQR::main(); }*/

/**********************************************************************************************/
/**********************************************************************************************/
/*
* Step1 Read QR code (hex)
    QR code(Hexa)	11109D6B2700A000200000E000000000
Step2 Convert the read QR code (hex) to binary
    When converting from hexadecimal to binary, convert one character (hex) to four characters (binary number).
                    1    1    1    0    9    D    6    B    2    7    0    0    A    0    0    0    2    0    0    0    0    0    E    0    0    0    0    0    0    0    0    0
    QR code(Binary)	0001 0001 0001 0000 1001 1101 0110 1011 0010 0111 0000 0000 1010 0000 0000 0000 0010 0000 0000 0000 0000 0000 1110 0000 0000 0000 0000 0000 0000 0000 0000 0000

    Step3 The result of converting to binary, separate the data for each item
                            start	length	Value(Binary)
    EncodeVersion			1		4		0001
    Print Area				5		4		0001
    Item Code				9		34		0001000010011101011010110010011100
    Packing Division		43		4		0000
    Production year			47		7		0010100
    Quantity				54		14		00000000000001
    Serial number			68		24		000000000000000000000111
    Blank					92		29		00000000000000000000000000000
    Check Sum				121		8		00000000

Step4 Convert delimited data to decimal
                            start	length	Value(Binary)							Value(Decimal)
    EncodeVersion (*1)		0		4		0001									2
    Print Area  (*2)		4		4		0001									1
    Item Code				8		34		0001000010011101011010110010011100		1115008156
    Packing Division (*3)	42		4		0000									0
    Production year			46		7		0010100									20
    Quantity				53		14		00000000000001							1
    Serial number (*4)		67		24		000000000000000000000111				7
    Blank					91		29		00000000000000000000000000000			0
    Check Sum				120		8		00000000								0


    *1  1 Fixed
    *2  1:Japan  2:America  3:China  4:France
    *3  0:piece  1:case
    *4  serial number?  Only 6 characters?
*/
namespace parseQR {

    using namespace std;

    string GetBinaryStringFromHexString(string);
    
    
    int main(int argc, char* argv[])
    {

        unsigned int encodeVer, printArea, itemCode, packingDiv, productionYear, quantity, serialNumber, checkSum;
        string inHex, inBinary;


        for (int i = 1; i < argc; ++i)
        {
            inHex = argv[i];
            inBinary = GetBinaryStringFromHexString(inHex);
            //cout << inHex << "\n";
            //cout << inBinary << "\n";

            encodeVer = strtoul(inBinary.substr(0, 4).c_str(), nullptr, 2);
            printArea = strtoul(inBinary.substr(4, 4).c_str(), nullptr, 2);
            itemCode = strtoul(inBinary.substr(8, 34).c_str(), nullptr, 2);
            packingDiv = strtoul(inBinary.substr(42, 4).c_str(), nullptr, 2);
            productionYear = strtoul(inBinary.substr(46, 7).c_str(), nullptr, 2);
            quantity = strtoul(inBinary.substr(53, 14).c_str(), nullptr, 2);
            serialNumber = strtoul(inBinary.substr(67, 24).c_str(), nullptr, 2);
            checkSum = strtoul(inBinary.substr(120, 8).c_str(), nullptr, 2);

            cout << "Encode Version: " << encodeVer << "\n";
            cout << "Print Area: " << printArea << "\n";
            cout << "Item Code: " << itemCode << "\n";
            cout << "Packing Division: " << packingDiv << "\n";
            cout << "Production Year: " << productionYear << "\n";
            cout << "Quantity: " << quantity << "\n";
            cout << "Serial Number: " << serialNumber << "\n";
            cout << "Checksum: " << checkSum << "\n";
        }


        return 0;
    }
    string GetBinaryStringFromHexString(string sHex)
    {
        string sReturn = "";
        for (unsigned int i = 0; i < sHex.length(); ++i)
        {
            switch (toupper(sHex[i]))
            {
            case '0': sReturn.append("0000"); break;
            case '1': sReturn.append("0001"); break;
            case '2': sReturn.append("0010"); break;
            case '3': sReturn.append("0011"); break;
            case '4': sReturn.append("0100"); break;
            case '5': sReturn.append("0101"); break;
            case '6': sReturn.append("0110"); break;
            case '7': sReturn.append("0111"); break;
            case '8': sReturn.append("1000"); break;
            case '9': sReturn.append("1001"); break;
            case 'A': sReturn.append("1010"); break;
            case 'B': sReturn.append("1011"); break;
            case 'C': sReturn.append("1100"); break;
            case 'D': sReturn.append("1101"); break;
            case 'E': sReturn.append("1110"); break;
            case 'F': sReturn.append("1111"); break;
            }
        }
        return sReturn;
    }
}
/**********************************************************************************************/
/**********************************************************************************************/
