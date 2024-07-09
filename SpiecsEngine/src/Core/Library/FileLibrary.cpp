/**
* @file FileLibrary.cpp
* @brief The FileLibrary Class Implementation.
* @author Travis Vroman.
*/

#include "Pchheader.h"
#include "FileLibrary.h"

#include <commdlg.h>

namespace Spiecs {

	bool FileLibrary::FileLibrary_Exists(const char* path)
	{
		struct _stat buffer {};
		return _stat (path, &buffer) == 0;
	}

	bool FileLibrary::FileLibrary_Open(const char* path, FileModes mode, bool binary, FileHandle* out_handle)
	{
		out_handle->is_valid = false;
		out_handle->handle   = nullptr;
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
			SPIECS_CORE_INFO("Invalid mode passed while trying to open file");
			return false;
		}

		FILE* file;
		auto state = fopen_s(&file, path, mode_str);
		
		if (!file) {
			SPIECS_CORE_INFO("Error opening file");
			return false;
		}

		out_handle->handle = file;
		out_handle->is_valid = true;

		return true;
	}

	void FileLibrary::FileLibrary_Close(FileHandle* handle)
	{
		if (handle->handle) {
			auto state = fclose(static_cast<FILE*>(handle->handle));
			handle->handle = nullptr;
			handle->is_valid = false;
		}
	}

	bool FileLibrary::FileLibrary_Size(const FileHandle* handle, uint64_t* out_size)
	{
		if (handle->handle) {
			auto state = fseek(static_cast<FILE*>(handle->handle), 0, SEEK_END);
			*out_size = ftell(static_cast<FILE*>(handle->handle));
			rewind(static_cast<FILE*>(handle->handle));	
			return true;
		}
		return false;
	}

	bool FileLibrary::FileLibrary_Read(const FileHandle* handle, uint64_t data_size, void* out_data, uint64_t* out_bytes_read)
	{
		if (handle->handle && out_data) {
			*out_bytes_read = fread(out_data, 1, data_size, static_cast<FILE*>(handle->handle));
			if (*out_bytes_read != data_size) {
				return false;
			}
			return true;
		}
		return false;
	}

	bool FileLibrary::FileLibrary_Read_Line(const FileHandle* handle, uint64_t max_length, char** line_buf, uint64_t* out_line_length)
	{
		if (handle->handle && line_buf && out_line_length && max_length > 0) {
			char* buf = *line_buf;
			if (fgets(buf, static_cast<int>(max_length), static_cast<FILE*>(handle->handle)) != nullptr) {
				*out_line_length = strlen(*line_buf);
				return true;
			}
		}
		return false;
	}

	bool FileLibrary::FileLibrary_Read_all_bytes(const FileHandle* handle, char* out_bytes, uint64_t* out_bytes_read)
	{
		if (handle->handle && out_bytes && out_bytes_read) {
			uint64_t size = 0;
			if (!FileLibrary_Size(handle, &size)) {
				return false;
			}

			*out_bytes_read = fread(out_bytes, 1, size, static_cast<FILE*>(handle->handle));
			return *out_bytes_read == size;
		}
		return false;
	}

	bool FileLibrary::FileLibrary_Write(const FileHandle* handle, uint64_t data_size, const void* data, uint64_t* out_bytes_written)
	{
		if (handle->handle) {
			*out_bytes_written = fwrite(data, 1, data_size, static_cast<FILE*>(handle->handle));
			if (*out_bytes_written != data_size) {
				return false;
			}
			auto state = fflush(static_cast<FILE*>(handle->handle));
			return true;
		}
		return false;
	}

	bool FileLibrary::FileLibrary_Write_Line(const FileHandle* handle, const char* text)
	{
		if (handle->handle) {
			int result = fputs(text, static_cast<FILE*>(handle->handle));
			if (result != EOF) {
				result = fputc('\n', static_cast<FILE*>(handle->handle));
			}

			// Make sure to flush the stream so it is written to the file immediately.
			// This prevents data loss in the event of a crash.
			auto state = fflush(static_cast<FILE*>(handle->handle));
			return result != EOF;
		}
		return false;
	}

	std::string FileLibrary::FileLibrary_OpenInExplore(const char* filter, HWND hwnd)
	{
		OPENFILENAMEA ofn;    // common dialog box structure
		CHAR szFile[260] = { 0 };   // if using TCHAR macros
		// Initialize OPEN FILENAME
		ZeroMemory(&ofn, sizeof(OPENFILENAMEA));
		ofn.lStructSize = sizeof(OPENFILENAMEA);
		ofn.hwndOwner = hwnd;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetOpenFileNameA(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}
		return "";
	}
}