/**
* @file FileLibrary.h
* @brief The FileLibrary Class Definitions.
* @author Travis Vroman.
*/

#pragma once
#include "Core/Core.h"

namespace Spiecs {

    /**
    * @brief This Struct is FILE* handle pointer Warpper.
    */
    struct FileHandle {

        /**
        * @brief FILE* handle.
        * Need cast while use.
        */
        void* handle;

        /**
        * @brief Is this hanled Vaild.
        */
        bool is_valid;
    };

    /**
    * @brief file mode
    */
    enum FileModes {

        /**
        * @brief model : read
        */
        FILE_MODE_READ = 0x1,

        /**
        * @brief model : write
        */
        FILE_MODE_WRITE = 0x2
    };

    /**
    * @brief File Static Function Library.
    */
    class FileLibrary
    {
    public:

        /**
        * @brief Determine whether the given string is existing a file.
        * @param[in] path The file path.
        * @return true if there exising a file.
        */
        static bool FileLibrary_Exists(const char* path);

        /**
        * @brief Open the file using given string.
        * @param[in] path The file path.
        * @param[in] mode The file mode.
        * @param[in] binary The file is a binary file ro not.
        * @param[out] out_handle Out The filehandle pointer.
        * @return true if open the file succeed.
        */
        static bool FileLibrary_Open(const char* path, FileModes mode, bool binary, FileHandle* out_handle);

        /**
        * @brief Close the file by the file handle.
        * @param[in] handle The file handle.
        */
        static void FileLibrary_Close(FileHandle* handle);

        /**
        * @brief Calculate The file size.
        * @param[in] handle The file handle.
        * @param[out] out_size The file size(byte).
        * @return true if file hanle is vaild.
        */
        static bool FileLibrary_Size(FileHandle* handle, uint64_t* out_size);

        /**
        * @brief Read Specific size of data form the current file handle pointer, and move pointer the same size.
        * @param[in] handle The file handle.
        * @param[in] data_size How much bytes we want read and move pointer.
        * @param[out] out_data The data we read.
        * @param[out] out_bytes_read How much bytes we readed.
        * @return true if file hanle is vaild and succesfully read data.
        */
        static bool FileLibrary_Read(FileHandle* handle, uint64_t data_size, void* out_data, uint64_t* out_bytes_read);

        /**
        * @brief Read one line from the current file handle pointer, and move pointer the same size.
        * @param[in] handle The file handle.
        * @param[in] max_length Defines the max size per line.
        * @param[out] line_buf The data of one line.
        * @param[out_line_length] out_bytes_read How much bytes we readed.
        * @return true if file hanle is vaild and succesfully read data.
        */
        static bool FileLibrary_Read_Line(FileHandle* handle, uint64_t max_length, char** line_buf, uint64_t* out_line_length);

        /**
        * @brief Read all data form the current file handle pointer.
        * @param[in] handle The file handle.
        * @param[out] out_bytes The data that read.
        * @param[out] out_bytes_read How much bytes we readed.
        * @return true if file hanle is vaild and succesfully read data.
        */
        static bool FileLibrary_Read_all_bytes(FileHandle* handle, char* out_bytes, uint64_t* out_bytes_read);

        /**
        * @brief Write given data to the file hanle pointer.
        * @param[in] handle The file handle.
        * @param[in] data_size How much bytes we want write and move pointer.
        * @param[in] data The data we want write in.
        * @param[out] out_bytes_written How much bytes we writtened.
        * @return true if file hanle is vaild and succesfully write data.
        */
        static bool FileLibrary_Write(FileHandle* handle, uint64_t data_size, const void* data, uint64_t* out_bytes_written);

        /**
        * @brief Write one line data to the file hanle pointer.
        * @param[in] handle The file handle.
        * @param[in] text The one line data pointer.
        * @return true if file hanle is vaild and succesfully write data.
        */
        static bool FileLibrary_Write_Line(FileHandle* handle, const char* text);


        static std::string FileLibrary_OpenInExplore(const char* filter, HWND hwnd);
    };
}