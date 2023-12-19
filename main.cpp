/*
 * Copyright 2023 Marcus Madland. All rights reserved.
 * License: https://github.com/MarcusMadland/ndtest/blob/main/LICENSE
 */

#include <stdio.h>

#include <mapp/commandline.h>
#include <mapp/file.h>

static bx::ErrorAssert s_errAssert;

struct PakHeader 
{													   
	uint32_t m_magic;								// 0x0 0x00000A79
	uint32_t m_hdrSize;								// 0x4 header size
	uint32_t m_pakLoginTableIdx;					// 0x8 idx of the page storing the PakLoginTable
	uint32_t m_pakLoginTableOffset;					// 0xC relative offset PakLoginTable = PakPageHeader + m_pakLoginTableOffset; //its a ResItem 
	uint32_t m_pageCt;								// 0x10 page count. Total number of pages in the package
	uint32_t m_pPakPageEntryTable;					// 0x14 ptr to the PakPageEntry array/table
	uint32_t m_numPointerFixUpPages;				// 0x18 always 0x8
	uint32_t m_pointerFixUpTableOffset;				// 0x1C ptr to the PointerFixUpTable table
	uint32_t m_dataSize;							// 0x20 size of the data from the end of the last page to the EOF
	uint32_t m_vramDescTablePageIdx;				// 0x24 page idx where the VramDescTable is stored
	uint32_t m_vramDescTableOffset;					// 0x28 m_pageHdrs[m_vramDescTablePageIdx] + m_vramDescTableOffset = ResItem
};													   
													   
struct PakPageEntry 								   
{													   
	uint32_t m_resPageOffset;						// 0x0 absolute offset to the ResPage
	uint32_t m_pageSize;							// 0x4 page size
	uint32_t m_owningPackageIndex;					// 0x8 
};													   
													   
struct ResPage 										   
{													   
	uint64_t m_unused;								// 0x0	0xDEADBEEFDEADBEEF
	uint32_t m_unk;									// 0x8	always 0 ?
	uint32_t m_pageSize;							// 0xC	size maybe from the start of PakPageHeader
	uint16_t m_unk2;								// 0x10
	uint16_t m_numPageHeaderEntries;				// 0x12	number of pointers in the table
};													   
													   
struct ResPageEntry 								   
{													   
	uint64_t m_name;								// 0x0 same as the ResItem name... This field is usually used by animations
	uint32_t m_resItemOffset;						// 0x8 relative offset. -> ResItem = PakPageHeader + m_pakLoginItemOffset
	uint32_t m_padding;								// 0xC always 00 01 23 45
};													   
													   
struct ResItem 										   
{													   
	uint64_t m_itemNameOffset;						// 0x0 item name. relative offset PakPageHeader + m_itemNameOffset;
	uint64_t m_itemTypeOffset;						// 0x8 item type name. relative offset PakPageHeader + m_itemTypeOffset; PM_COLLECTION_2 || RAW_DATA etc
	uint32_t m_unkNumber;							// 0x10 no idea what this is used for seems to vary from entry to entry
	uint32_t m_unused;								// 0x14 always 00 01 23 45 
	uint32_t m_unused2;								// 0x18 always 00 01 23 45 NOTE: this might be used by the JOINT_HIERARCHY || ANIM
	uint32_t m_unused3;								// 0x1C always 00 01 23 45 NOTE: this might be used by the JOINT_HIERARCHY || ANIM
};

struct PointerFixUpPage 
{
	uint32_t m_pageEntryNumber;						// 0x0 0, 1, 2 ...
	uint32_t m_dataOffset;							// 0x4 ptr to the table
	uint32_t m_numLoginPageEntries;					// 0x8 number of login pages entries
};

struct PointerFixUpPageEntry 
{
	uint16_t m_page1Idx;							// 0x0 idx in the page table
	uint16_t m_page2Idx;							// 0x2 idx in the page table
	uint32_t m_offset;								// 0x4 offset
};

struct PakLoginTableEntry 
{
	uint32_t m_wantedPage;							// 0x0 page idx in the table
	uint32_t m_resItemOffset;						// 0x4 offset of the res item
};

int main(int _argc, const char* const* _argv)
{
	bx::CommandLine cmdLine(_argc, (const char**)_argv);

	// Manage input
	const char* input = cmdLine.findOption("-i");
	if (NULL == input)
	{
		input = "C:/Users/marcu/Dev/uncharted-extracted/Uncharted4_data/build/pc/uncharted4/pak54/anim-player-movement-relaxed-level.pak";
	}
	bx::printf("Reading file at path: %s\n", input);

	// Open file
	bx::FileReader reader;
	if (bx::open(&reader, input, s_errAssert))
	{
		// Read header
		PakHeader header;
		bx::read(&reader, &header.m_magic, sizeof(header.m_magic), s_errAssert);
		if (header.m_magic == 0xA79 || header.m_magic == 0x10A79) // Normal pak or -dict.pak
		{
			bx::seek(&reader, 0, bx::Whence::Begin);
			bx::read(&reader, &header, sizeof(header), s_errAssert);
		}
		else
		{
			bx::printf("File [%s] is an older version, found 0x%08x instead of %s!\n", input, header.m_magic, "0xA79 / 0x10A79");
			return 0;
		}

		// Read page
		for (U32 i = 0; i < header.m_pageCt; i++)
		{
			ResPage page;
			bx::read(&reader, &page, sizeof(page), s_errAssert);

			// Read entries
			for (U32 i = 0; i < page.m_numPageHeaderEntries; i++)
			{
				ResPageEntry entry;
				bx::read(&reader, &entry, sizeof(entry), s_errAssert);
			}
		}
		
		
		// Close file
		bx::close(&reader);
	}

	while (getchar() != '\n') {} return bx::kExitSuccess;
}