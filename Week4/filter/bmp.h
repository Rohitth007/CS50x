// BMP-related data types based on Microsoft's own
// Best Article on the Topic:
// https://itnext.io/bits-to-bitmaps-a-simple-walkthrough-of-bmp-image-format-765dc6857393

#include <stdint.h>

/**
 * Common Data Types
 *
 * The data types in this section are essentially aliases for C/C++
 * primitive data types.
 *
 * Adapted from http://msdn.microsoft.com/en-us/library/cc230309.aspx.
 * See http://en.wikipedia.org/wiki/Stdint.h for more on stdint.h.
 */
typedef uint8_t  BYTE;
typedef uint32_t DWORD;
typedef int32_t  LONG;
typedef uint16_t WORD;

/**
 * BITMAPFILEHEADER
 *
 * The BITMAPFILEHEADER structure contains information about the type, size,
 * and layout of a file that contains a DIB [device-independent bitmap].
 *
 * Adapted from http://msdn.microsoft.com/en-us/library/dd183374(VS.85).aspx.
 */
typedef struct
{
    WORD   bfType;             // 2 bytes char'BM' or '0x4d42' in hex(little endian)
    DWORD  bfSize;             // 4 bytes for size of whole BMP file.
    WORD   bfReserved1;        // 2 bytes reserved for image processing application
    WORD   bfReserved2;        // 2 bytes --ditto--
    DWORD  bfOffBits;          // 4 bytes number of bytes till the first byte of RGBTriple which is usually 54 (14+40)
} __attribute__((__packed__))
BITMAPFILEHEADER;              // 14 bytes total

/**
 * BITMAPINFOHEADER
 *
 * The BITMAPINFOHEADER structure contains information about the
 * dimensions and color format of a DIB [device-independent bitmap].
 *
 * Adapted from http://msdn.microsoft.com/en-us/library/dd183376(VS.85).aspx.
 */
typedef struct
{
    DWORD  biSize;               // 4 bytes sizeof(BITMAPINFOHEADER) = 40 bytes.
    LONG   biWidth;              // 4 byted width in number of pixels.
    LONG   biHeight;             // 4 bytes height in number of pixels.
    WORD   biPlanes;             // 2 bytes represents number of ColourPlanes, set to '1'.
    WORD   biBitCount;           // 2 bytes bits per pixel (24 bits for RGB-24bit-bitmap).
    DWORD  biCompression;        // 4 bytes The value of compression to use, set to '0' for BMP.
    DWORD  biSizeImage;          // 4 bytes size of compressed image, set to '0' for BMP.
    LONG   biXPelsPerMeter;      // 4 bytes represents horizontal resolution, set to '0' for no preference.
    LONG   biYPelsPerMeter;      // 4 bytes -----------vertical   ---------------ditto--------------------.
    DWORD  biClrUsed;            // 4 bytes total number of colors used in collor pallet (sizeof(colorpallet)). If set to '0' the 2^biBitCount colours used.
    DWORD  biClrImportant;       // 4 bytes number of important colours used. ignored by setting to '0'.
} __attribute__((__packed__))
BITMAPINFOHEADER;                // 40 bytes total

/**
 * RGBTRIPLE
 *
 * This structure describes a color consisting of relative intensities of
 * red, green, and blue.
 *
 * Adapted from http://msdn.microsoft.com/en-us/library/aa922590.aspx.
 */
typedef struct
{
    BYTE  rgbtBlue;
    BYTE  rgbtGreen;
    BYTE  rgbtRed;
} __attribute__((__packed__))
RGBTRIPLE;
