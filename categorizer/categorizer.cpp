#include "categorizer.h"

using namespace std;

int ExtractNum(fstream* file, const char delimiter)
{
    int alphabet[256];
    char sym;
    int res = 0;
    unsigned int digit_b = static_cast<unsigned int>(file->tellg());

    do
    {
        file->read(&sym, 1);
    } while (sym != delimiter);

    unsigned int digit_e = static_cast<unsigned int>(file->tellg()) - 2;
    int j = 0;
    int c = 1;

    for (int d = '0'; d <= '9'; d++, j++) 
        alphabet[d] = j;
    
    for (j = digit_e; j >= digit_b; j--, c *= 10)
    {
        file->seekg(j);
        file->read(&sym, 1);
        res += alphabet[sym] * c;
    }
    file->seekg(digit_e + 2);
    return res;
}

void BuildHistogram(std::fstream* file, unsigned int* histogram, unsigned int& len, int& width,  int& height)
{
    char sym;
    for (unsigned int d = 0; d < len; ++d)
        histogram[d] = 0;

    file->seekg(static_cast<unsigned int>(file->tellg()) + 4);

    for (unsigned int d = 0; d < (width * height); ++d)
    {
        if (file->eof())break;
        file->read(&sym, 1);
        histogram[static_cast<unsigned char>(sym)] += 1;
    }
    cout << endl;
}

void PrintHistogram(unsigned int* histogram, unsigned int& len, int& width, int& height)
{
    ofstream* out = new ofstream("./histogram.txt");
    *out << "Building histogram for graphic file" << endl;
    *out << "Width: " << width << "\t Height: " << height << endl;
    for (unsigned int j = 0; j < len; j++)
    {
        *out << "[" << j << "] ";
        for (unsigned int c = 0; c < histogram[j]; c++)
            *out << "|";
        *out << endl;
    }
    *out << "End of file " << endl << endl;
    out->close();
    delete out;
}

void ProcessFile(std::fstream* file, unsigned int* histogram, unsigned int len)
{
    int width = 0, height = 0;
    char PGM_hdr1[2];

    file->read(PGM_hdr1, 2);

    if ((PGM_hdr1[0] != 0x50) || (PGM_hdr1[1] != 0x35)) //чек заголовка файла
        return;

    cout << "PGM header detected" << endl;

    file->seekg(static_cast<unsigned int>(file->tellg()) + 1);  //смещение в потоке

    width = ExtractNum(file, 0x20);
    cout << "Width: " << width << endl;

    height = ExtractNum(file, 0x0A);
    cout << "Height: " << width << endl;

    BuildHistogram(file, histogram, len, width, height);
    PrintHistogram(histogram, len, width, height);
}

int main(int argc, char** args)
{
    fstream *f = new fstream();
    unsigned int histogram[256];

    f->open("./splash.pgm", ios::binary | ios::in);

    if (!f->is_open())
        return 1;

    ProcessFile(f, histogram, 256);
    f->close();

    delete f;
    return 0;
}
