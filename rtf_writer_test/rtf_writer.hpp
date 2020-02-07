#pragma once



/////////////////////////////////////////////////////////////////////////////
//==========================================================================
#include <string>
#include <map>
#include <vector>
#include <algorithm>

#include <sstream>
#include <iomanip>


//==========================================================================
#include "rtf_define.hpp"
#include "rtf_structures.hpp"





/////////////////////////////////////////////////////////////////////////////
//==========================================================================
// 
// http://www.biblioscape.com/rtf15_spec.htm
// https://www.codeproject.com/Articles/10582/rtflib-v1-0
//





/////////////////////////////////////////////////////////////////////////////
//==========================================================================
class rtf_writer
{
public:
	std::ostringstream _oss;

public:
	RTF_DOCUMENT_FORMAT  _DocFormat;  // RTF document formatting params
	RTF_SECTION_FORMAT   _SecFormat;  // RTF section formatting params
	RTF_PARAGRAPH_FORMAT _ParFormat;  // RTF paragraph formatting params
	RTF_TABLEROW_FORMAT  _RowFormat;  // RTF table row formatting params
	RTF_TABLECELL_FORMAT _CellFormat; // RTF table cell formatting params

	// RTF library global params
	FILE* _File;
	std::string _FontTable ;
	std::string _ColorTable;
	std::string _ParagraphText;

public:
	rtf_writer();
	~rtf_writer();

public:
	int                   open                 (char* filename, char* fonts, char* colors);    // Creates new RTF document
	int                   close                ();                                             // Closes created RTF document
	bool                  write_header         ();                                             // Writes RTF document header
	void                  init                 ();                                             // Sets global RTF library params
	void                  set_fonttable        (char* fonts);                                  // Sets new RTF document font table
	void                  set_colortable       (char* colors);                                 // Sets new RTF document color table
	RTF_DOCUMENT_FORMAT*  get_documentformat   ();                                             // Gets RTF document formatting properties
	void                  set_documentformat   (RTF_DOCUMENT_FORMAT* df);                      // Sets RTF document formatting properties
	bool                  write_documentformat ();                                             // Writes RTF document formatting properties
	RTF_SECTION_FORMAT*   get_sectionformat    ();                                             // Gets RTF section formatting properties
	void                  set_sectionformat    (RTF_SECTION_FORMAT* sf);                       // Sets RTF section formatting properties
	bool                  write_sectionformat  ();                                             // Writes RTF section formatting properties
	int                   start_section        ();                                             // Starts new RTF section
	RTF_PARAGRAPH_FORMAT* get_paragraphformat  ();                                             // Gets RTF paragraph formatting properties
	void                  set_paragraphformat  (RTF_PARAGRAPH_FORMAT* pf);                     // Sets RTF paragraph formatting properties
	bool                  write_paragraphformat();                                             // Writes RTF paragraph formatting properties
	int                   start_paragraph      (char* text, bool newPar=false);                // Starts new RTF paragraph
	int                   load_image           (char* image_file_path, int width, int height); // Loads image from file
	std::string           binary_to_hex        (unsigned char* binary, int size);              // Converts binary data to hex
	void                  set_defaultformat    ();                                             // Sets default RTF document formatting
	int                   start_tablerow       ();                                             // Starts new RTF table row
	int                   end_tablerow         ();                                             // Ends RTF table row
	int                   start_tablecell      (int rightMargin);                              // Starts new RTF table cell
	int                   end_tablecell        ();                                             // Ends RTF table cell
	RTF_TABLEROW_FORMAT*  get_tablerowformat   ();                                             // Gets RTF table row formatting properties
	void                  set_tablerowformat   (RTF_TABLEROW_FORMAT* rf);                      // Sets RTF table row formatting properties
	RTF_TABLECELL_FORMAT* get_tablecellformat  ();                                             // Gets RTF table cell formatting properties
	void                  set_tablecellformat  (RTF_TABLECELL_FORMAT* cf);                     // Sets RTF table cell formatting properties
	std::string           get_bordername       (int border_type);                              // Gets border name
	std::string           get_shadingname      (int shading_type, bool cell);                  // Gets shading name
};


