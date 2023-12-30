#pragma once
#include "_bite_stream_frame.h"
#include "_bite_dye.h"
#include "_bite_span.h"
#include "_bite_array_stack.h"
#include "_bite_common_passwords.h"
#include "_bite_reporting.h"
#include "_bite_algorithms.h"
#include "_bite_str.h"
#include "_bite_net.h"
#include "_bite_file.h"
#include "_bite_memorypool.h"
#include "_bite_stackptr.h"



namespace bite
{
	namespace dye
	{
	} // dye

	typedef struct
	{
		uint8_t major, minor, patch;
	}Version_t;

	extern constexpr Version_t get_version();


} // bite

