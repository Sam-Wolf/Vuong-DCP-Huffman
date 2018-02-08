#define DllExport   __declspec( dllexport )  

// Encode Function
extern "C" DllExport  unsigned int Encode(const unsigned char * __restrict in, unsigned char * __restrict out, const unsigned int length);

// Decode Funktion
extern "C" DllExport void Decode(const unsigned char * __restrict in, unsigned char * __restrict out, const unsigned int length);

// Compression Method ID
extern "C" DllExport unsigned long  GetCompressDWORD() {
	return 'HUFF';
};

// Compression Method Name 
extern "C"  DllExport const char * GetCompressName() {
	return "Huffman";
};

// returns if a bigger output buffer is needed
extern "C" DllExport bool GetBigBuffer() {
	return 0;
};

