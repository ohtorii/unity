#pragma once
#include<stdint.h>


enum class UTF_BOM_TYPE :int {
	UTF8 = 0,
	UTF16LE,
	UTF16BE,
	UNKNOWN,
};

UTF_BOM_TYPE GetBomType(const uint8_t*buffer, size_t buffer_byte_size);
size_t GetBomSize(UTF_BOM_TYPE bom_type);

