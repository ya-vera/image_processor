#include "bmpfile.h"

typedef struct {
//    uint8_t  Signature[2];
    uint32_t FileSize;
    uint16_t Reserved[2];
    uint32_t PixelArrayOffset;
} BitmapFileHeader;

typedef struct {
    uint32_t HeaderSize;
    int32_t  Width;
    int32_t  Height;
    uint16_t ColorPlanes;
    uint16_t BPP;
    uint32_t Compression;
    uint32_t ImageSize;
    int32_t  Resolution_H;
    int32_t  Resolution_V;
    uint32_t NumberOfColors;
    uint32_t ImportantColors;
} BitmapInfoHeader;




bool LoadBitmap(std::ifstream &file, Image &image)
{
    if (!file.good()) return false; // Bitmap not loaded
    char Signature[2];
    file.read(Signature, 2);
    if ((Signature[0] != 'B')||(Signature[1] != 'M')) return false;  // Wrong signature
    BitmapFileHeader fileHeader;
    file.read((char*)&fileHeader, sizeof(BitmapFileHeader));
    if (!file.good()) return false; // Bitmap not loaded
    BitmapInfoHeader infoHeader;
    file.read((char*)&infoHeader, sizeof(BitmapInfoHeader));
    if (!file.good()) return false; // Bitmap not loaded
    if (infoHeader.HeaderSize != sizeof(BitmapInfoHeader)) return false; // Wrong header
    if (infoHeader.ColorPlanes != 1) return false;
    if (infoHeader.BPP != 24) return false; //Wrong BPP
    if (infoHeader.Compression != 0) return false; // Wrong Compression
    if (infoHeader.Width <= 0) return false;
    if (infoHeader.Height == 0) return false;

    image.SetSize(infoHeader.Width, abs(infoHeader.Height));

    int rowSize = (infoHeader.Width * infoHeader.BPP + 31)/32 * 4;

    for (int i=0;i<image.Height;i++)
    {
        file.seekg(fileHeader.PixelArrayOffset + rowSize * i);
        int row = (infoHeader.Height > 0) ? (infoHeader.Height - i - 1) : i;
        if (!file.good()) return false;
        for (int j=0;j<image.Width;j++)
        {
            unsigned char RGB[3];
            file.read((char*)RGB, 3);
            if (!file.good())
            {
                return false;
            }
            image.Pixels[j][row].setBmpVal(IDX_B, RGB[0]);
            image.Pixels[j][row].setBmpVal(IDX_G, RGB[1]);
            image.Pixels[j][row].setBmpVal(IDX_R, RGB[2]);
        }
    }
    return true;
}

bool SaveBitmap(std::ofstream &file, Image &image)
{
    int rowSize = (image.Width * 24 + 31)/32 * 4;
    int rowExtra = rowSize - image.Width * 3;
    int imageSize = rowSize * image.Height;

    char Signature[2] = {'B', 'M'};
    BitmapFileHeader fileHeader;
    fileHeader.FileSize = 2 + sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + imageSize;
    fileHeader.PixelArrayOffset = 2 + sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);
    fileHeader.Reserved[0] = 0;
    fileHeader.Reserved[1] = 0;
    BitmapInfoHeader infoHeader;
    infoHeader.HeaderSize = sizeof(BitmapInfoHeader);
    infoHeader.Width = image.Width;
    infoHeader.Height = image.Height;
    infoHeader.ColorPlanes = 1;
    infoHeader.BPP = 24;
    infoHeader.Compression = 0;
    infoHeader.ImageSize = imageSize;
    infoHeader.Resolution_H = 0; //11811; // 300 dpi
    infoHeader.Resolution_V = 0; //11811; // 300 dpi
    infoHeader.NumberOfColors = 0;
    infoHeader.ImportantColors = 0;
    file.write(Signature, 2);
    file.write((const char*)&fileHeader, sizeof(BitmapFileHeader));
    file.write((const char*)&infoHeader, sizeof(BitmapInfoHeader));

    for (int i=0;i<image.Height;i++)
    {
        int row = image.Height - 1 - i;
        for (int j=0;j<image.Width;j++)
        {
            unsigned char RGB[3];
            RGB[0] = image.Pixels[j][row].getBmpVal(IDX_B);
            RGB[1] = image.Pixels[j][row].getBmpVal(IDX_G);
            RGB[2] = image.Pixels[j][row].getBmpVal(IDX_R);
            file.write((const char*)RGB, 3);
        }
        char pad = 0;
        for (int j=0;j<rowExtra;j++)
            file.write(&pad, 1);
    }
    return file.good();
}
