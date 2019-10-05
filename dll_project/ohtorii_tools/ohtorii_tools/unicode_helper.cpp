#include"stdafx.h"


UTF_BOM_TYPE GetBomType(const uint8_t*buffer, size_t buffer_byte_size) {
	if (buffer_byte_size < 2) {
		return UTF_BOM_TYPE::UNKNOWN;
	}

	if ((buffer[0] == 0xFF) and (buffer[1] == 0xFE)) {
		return UTF_BOM_TYPE::UTF16LE;
	}
	if ((buffer[0] == 0xFE) and (buffer[1] == 0xFF)) {
		return UTF_BOM_TYPE::UTF16BE;
	}

	if (buffer_byte_size < 3) {
		return UTF_BOM_TYPE::UNKNOWN;
	}
	if ((buffer[0] == 0xEF) and (buffer[1] == 0xBB) and (buffer[2] == 0xBF)) {
		return UTF_BOM_TYPE::UTF8;
	}

	return UTF_BOM_TYPE::UNKNOWN;
}

size_t GetBomSize(UTF_BOM_TYPE bom_type) {
	switch (bom_type) {
	case UTF_BOM_TYPE::UTF8:		return 3;
	case UTF_BOM_TYPE::UTF16LE:		return 2;
	case UTF_BOM_TYPE::UTF16BE:		return 2;
	case UTF_BOM_TYPE::UNKNOWN:
		//pass
	default:
		break;
	}
	return 0;
}


