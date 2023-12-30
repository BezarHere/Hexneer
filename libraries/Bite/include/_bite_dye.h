#pragma once
#include "_bite_base.h"
#include <iostream>
#include <string>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef WIN32_LEAN_AND_MEAN

namespace bite
{
	namespace dye
	{
#ifdef _WINDOWS_

		enum class ColorCode : uint8_t
		{
			Black,
			Blue,
			Green,
			Aqua,
			Red,
			Purple,
			Yellow,
			White,
			Gray,
			LightBlue,
			LightGreen,
			LightAqua,
			LightRed,
			LightPurple,
			LightYellow,
			BrightWhite
		};

		struct TerminalColor {
			ColorCode fg, bg = ColorCode::Black;
		};

		static constexpr TerminalColor g_DefaultTermColor{ ColorCode::White, ColorCode::Black };
		static TerminalColor g_current_color{ g_DefaultTermColor };

		inline constexpr uint8_t unpack_colors(TerminalColor theme)
		{
			return (uint8_t)theme.fg + ((uint8_t)theme.bg << 4);
		}

		inline constexpr TerminalColor pack_colors(uint8_t theme)
		{
			return { (ColorCode)(theme & 0xf), (ColorCode)(theme >> 4) };
		}

		inline TerminalColor get_colors()
		{
			return g_current_color;
		}


		inline TerminalColor get_default_colors()
		{
			return g_DefaultTermColor;
		}

		inline void put_colors(TerminalColor theme)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)unpack_colors(theme));
			g_current_color = theme;
		}

		// will return to the same theme the program started with
		inline void clear_colors()
		{
			put_colors(g_DefaultTermColor);
		}

		inline void dye(const std::string &text,
											const TerminalColor colors = TerminalColor{ ColorCode::BrightWhite },
											std::ostream &stream = std::cout)
		{
			TerminalColor def_t = get_colors();
			put_colors(colors);
			stream << text;
			put_colors(def_t);
		}

		inline void dye(const char *text,
											const TerminalColor colors = TerminalColor{ ColorCode::BrightWhite },
											std::ostream &stream = std::cout)
		{
			TerminalColor def_t = get_colors();
			put_colors(colors);
			stream << text;
			put_colors(def_t);
		}

		inline void dye(const std::exception &exc,
											const TerminalColor colors = TerminalColor{ ColorCode::Red },
											std::ostream &stream = std::cout)
		{
			TerminalColor def_t = get_colors();
			put_colors(colors);
			stream << exc.what() << std::endl;
			put_colors(def_t);
		}
#endif
	}

}
