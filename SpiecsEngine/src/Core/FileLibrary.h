#pragma once
#include "Core.h"
#include "vector"

namespace Spiecs {

    struct FileHandle {
        void* handle;
        bool is_valid;
    };

    enum FileModes {
        FILE_MODE_READ = 0x1,
        FILE_MODE_WRITE = 0x2
    };

    class FileLibrary
    {
    public:
        FileLibrary() = default;
        virtual ~FileLibrary() = default;

        static bool FileLibrary_Exists(const char* path);
        static bool FileLibrary_Open(const char* path, FileModes mode, bool binary, FileHandle* out_handle);
        static void FileLibrary_Close(FileHandle* handle);
        static bool FileLibrary_Size(FileHandle* handle, uint64_t* out_size);
        static bool FileLibrary_Read(FileHandle* handle, uint64_t data_size, void* out_data, uint64_t* out_bytes_read);
        static bool FileLibrary_Read_all_bytes(FileHandle* handle, char* out_bytes, uint64_t* out_bytes_read);
        static bool FileLibrary_Write(FileHandle* handle, uint64_t data_size, const void* data, uint64_t* out_bytes_written);

    };
}