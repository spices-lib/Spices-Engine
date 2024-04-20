/**
* @file FileLibrary.cpp
* @brief The FileLibrary Class Implementation.
* @author Travis Vroman.
*/

#include "Pchheader.h"
#include "FileLibrary.h"

namespace Spiecs {

	bool FileLibrary::FileLibrary_Exists(const char* path)
	{
		struct _stat buffer;
		return _stat(path, &buffer) == 0;
	}

	bool FileLibrary::FileLibrary_Open(const char* path, FileModes mode, bool binary, FileHandle* out_handle)
	{
		out_handle->is_valid = false;
		out_handle->handle = 0;
		const char* mode_str;

		if ((mode & FILE_MODE_READ) != 0 && (mode & FILE_MODE_WRITE) != 0) {
			mode_str = binary ? "w+b" : "w+";
		}
		else if ((mode & FILE_MODE_READ) != 0 && (mode & FILE_MODE_WRITE) == 0) {
			mode_str = binary ? "rb" : "r";
		}
		else if ((mode & FILE_MODE_READ) == 0 && (mode & FILE_MODE_WRITE) != 0) {
			mode_str = binary ? "wb" : "w";
		}
		else {
			SPIECS_LOG("Invalid mode passed while trying to open file");
			return false;
		}

		FILE* file = fopen(path, mode_str);
		if (!file) {
			SPIECS_LOG("Error opening file");
			return false;
		}

		out_handle->handle = file;
		out_handle->is_valid = true;

		return true;
	}

	void FileLibrary::FileLibrary_Close(FileHandle* handle)
	{
		if (handle->handle) {
			fclose((FILE*)handle->handle);
			handle->handle = 0;
			handle->is_valid = false;
		}
	}

	bool FileLibrary::FileLibrary_Size(FileHandle* handle, uint64_t* out_size)
	{
		if (handle->handle) {
			fseek((FILE*)handle->handle, 0, SEEK_END);
			*out_size = ftell((FILE*)handle->handle);
			rewind((FILE*)handle->handle);	
			return true;
		}
		return false;
	}

	bool FileLibrary::FileLibrary_Read(FileHandle* handle, uint64_t data_size, void* out_data, uint64_t* out_bytes_read)
	{
		if (handle->handle && out_data) {
			*out_bytes_read = fread(out_data, 1, data_size, (FILE*)handle->handle);
			if (*out_bytes_read != data_size) {
				return false;
			}
			return true;
		}
		return false;
	}

	bool FileLibrary::FileLibrary_Read_Line(FileHandle* handle, uint64_t max_length, char** line_buf, uint64_t* out_line_length)
	{
		if (handle->handle && line_buf && out_line_length && max_length > 0) {
			char* buf = *line_buf;
			if (fgets(buf, (int)max_length, (FILE*)handle->handle) != 0) {
				*out_line_length = strlen(*line_buf);
				return true;
			}
		}
		return false;
	}

	bool FileLibrary::FileLibrary_Read_all_bytes(FileHandle* handle, char* out_bytes, uint64_t* out_bytes_read)
	{
		if (handle->handle && out_bytes && out_bytes_read) {
			uint64_t size = 0;
			if (!FileLibrary_Size(handle, &size)) {
				return false;
			}

			*out_bytes_read = fread(out_bytes, 1, size, (FILE*)handle->handle);
			return *out_bytes_read == size;
		}
		return false;
	}

	bool FileLibrary::FileLibrary_Write(FileHandle* handle, uint64_t data_size, const void* data, uint64_t* out_bytes_written)
	{
		if (handle->handle) {
			*out_bytes_written = fwrite(data, 1, data_size, (FILE*)handle->handle);
			if (*out_bytes_written != data_size) {
				return false;
			}
			fflush((FILE*)handle->handle);
			return true;
		}
		return false;
	}

	bool FileLibrary::FileLibrary_Write_Line(FileHandle* handle, const char* text)
	{
		if (handle->handle) {
			int result = fputs(text, (FILE*)handle->handle);
			if (result != EOF) {
				result = fputc('\n', (FILE*)handle->handle);
			}

			// Make sure to flush the stream so it is written to the file immediately.
			// This prevents data loss in the event of a crash.
			fflush((FILE*)handle->handle);
			return result != EOF;
		}
		return false;
	}
}