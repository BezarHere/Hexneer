#pragma once
#include <string>

namespace bite
{
	namespace filesys
	{
		typedef struct stat info_t;
		typedef uint32_t DeviceId_t;
		enum DirFlags : unsigned short
		{
			IsValid = 0x0001,
			IsDir = _S_IFDIR,
			SpecialChar = _S_IFCHR,
			Link = _S_IFIFO,
			ReadPermission = _S_IREAD,
			WritePermission = _S_IWRITE,
			ExcutePermission = _S_IEXEC,
		};

		struct DirStat
		{
			DirStat(const std::string &path, const info_t &&info);

			DirStat(const std::string &path);
			DirStat(const char *const path);

			DirStat(const DirStat &copy);

			// can copy but can't assign
			DirStat operator =(const DirStat &copy) = delete;

			// cheking only if the paths match
			inline bool operator==(const DirStat &copy)
			{
				return filepath == copy.filepath;
			}

			inline bool isValid() const
			{
				return flags & DirFlags::IsValid;
			}

			inline bool isDir() const
			{
				return flags & DirFlags::IsDir;
			}

			inline bool isFile() const
			{
				return !(flags & DirFlags::IsDir);
			}

			inline char getDriverChar() const
			{
				// not ideal
				return 'A' + (char)device;
			}


			const std::string filepath;
			const DirFlags flags;
			const DeviceId_t device;
		};

		extern bool exists(const std::string &path);
		extern bool exists(const char *const path);

		extern size_t size(const std::string &path);
		extern size_t size(const char *const path);

		extern bool isdir(const std::string &path);
		extern bool isdir(const char *const path);
	}

}
