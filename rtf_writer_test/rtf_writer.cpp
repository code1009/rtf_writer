#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <io.h>
#include <olectl.h>

#include "rtf_writer.hpp"


IPicture* _Picture = NULL;



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
rtf_writer::rtf_writer()
{
	_File    = NULL;
	_Picture = NULL;
}


rtf_writer::~rtf_writer()
{
}

// Creates new RTF document
int rtf_writer::open( char* filename, char* fonts, char* colors )
{
	// Set error flag
	int error = RTF_SUCCESS;

	// Initialize global params
	init();
#if 0
	// Set RTF document font table
	if ( fonts != NULL )
	{
		if ( strcmp( fonts, "" ) != 0 )
			set_fonttable(fonts);
	}

	// Set RTF document color table
	if ( colors != NULL )
	{
		if ( strcmp( colors, "" ) != 0 )
			set_colortable(colors);
	}
#endif
	// Create RTF document
	_File = fopen( filename, "w" );

	if ( _File != NULL )
	{
		// Write RTF document header
		if ( !write_header() )
			error = RTF_HEADER_ERROR;

		// Write RTF document formatting properties
		if ( !write_documentformat() )
			error = RTF_DOCUMENTFORMAT_ERROR;

		// Create first RTF document section with default formatting
		write_sectionformat();
	}
	else
		error = RTF_OPEN_ERROR;

	// Return error flag
	return error;
}


// Closes created RTF document
int rtf_writer::close()
{
	// Set error flag
	int error = RTF_SUCCESS;

	// Free IPicture object
	if ( _Picture != NULL )
	{
		_Picture->Release();
		_Picture = NULL;
	}

	// Write RTF document end part
	std::ostringstream rtfText;

	rtfText << "\n\\par}";
	fwrite( rtfText.str().c_str(), 1, rtfText.str().size(), _File );

	// Close RTF document
	if ( fclose(_File) )
		error = RTF_CLOSE_ERROR;

	// Return error flag
	return error;
}


// Writes RTF document header
bool rtf_writer::write_header()
{
	// Set error flag
	bool result = true;

	// Standard RTF document header
	std::ostringstream rtfText;
	rtfText 
		<< "{\\rtf1\\ansi\\ansicpg1252\\deff0{\\fonttbl"
		<< _FontTable
		<< "}{\\colortbl"
		<< _ColorTable
		<< "}{\\*\\generator rtflib ver. 1.0;}"
		<< "\n{\\info{\\author rtflib ver. 1.0}{\\company ETC Company LTD.}}"
		;

	// Writes standard RTF document header part
	fwrite( rtfText.str().c_str(), 1, rtfText.str().size(), _File );

	// Return error flag
	return result;
}


// Sets global RTF library params
void rtf_writer::init()
{
	std::ostringstream rtfText;

	// Set RTF document default font table
	rtfText.str("");
	rtfText 
		<< "{\\f0\\froman\\fcharset0\\cpg1252 Times New Roman}"
		<< "{\\f1\\fswiss\\fcharset0\\cpg1252 Arial}"
		<< "{\\f2\\fmodern\\fcharset0\\cpg1252 Courier New}"
		<< "{\\f3\\fscript\\fcharset0\\cpg1252 Cursive}"
		<< "{\\f4\\fdecor\\fcharset0\\cpg1252 Old English}"
		<< "{\\f5\\ftech\\fcharset0\\cpg1252 Symbol}"
		<< "{\\f6\\fbidi\\fcharset0\\cpg1252 Miriam}"
//		   "{\\f%d\\fnil\\fcharset0\\cpg1252 %s}", font_number, token );
		;
	_FontTable = rtfText.str();

	// Set RTF document default color table
	rtfText.str("");
	rtfText
		<< "\\red0\\green0\\blue0;"
		<< "\\red255\\green0\\blue0;"
		<< "\\red0\\green255\\blue0;"
		<< "\\red0\\green0\\blue255;"
		<< "\\red255\\green255\\blue0;"
		<< "\\red255\\green0\\blue255;"
		<< "\\red0\\green255\\blue255;"
		<< "\\red255\\green255\\blue255;"
		<< "\\red128\\green0\\blue0;"
		<< "\\red0\\green128\\blue0;"
		<< "\\red0\\green0\\blue128;"
		<< "\\red128\\green128\\blue0;"
		<< "\\red128\\green0\\blue128;"
		<< "\\red0\\green128\\blue128;"
		<< "\\red128\\green128\\blue128;"
		;
	_ColorTable = rtfText.str();

	// Set default formatting
	set_defaultformat();
}


// Sets default RTF document formatting
void rtf_writer::set_defaultformat()
{
	// Set default RTF document formatting properties
	RTF_DOCUMENT_FORMAT df = {RTF_DOCUMENTVIEWKIND_PAGE, 100, 12240, 15840, 1800, 1800, 1440, 1440, false, 0, false};
	set_documentformat(&df);

	// Set default RTF section formatting properties
	RTF_SECTION_FORMAT sf = {RTF_SECTIONBREAK_CONTINUOUS, false, true, 12240, 15840, 1800, 1800, 1440, 1440, 0, 720, 720, false, 720, 720, false, 1, 720, false};
	set_sectionformat(&sf);

	// Set default RTF paragraph formatting properties
	RTF_PARAGRAPH_FORMAT pf = {RTF_PARAGRAPHBREAK_NONE, false, true, RTF_PARAGRAPHALIGN_LEFT, 0, 0, 0, 0, 0, 0, "", false, false, false, false, false, false};
	pf.BORDERS.borderColor = 0;
	pf.BORDERS.borderKind  = RTF_PARAGRAPHBORDERKIND_NONE;
	pf.BORDERS.borderSpace = 0;
	pf.BORDERS.borderType  = RTF_PARAGRAPHBORDERTYPE_STHICK;
	pf.BORDERS.borderWidth = 0;
	pf.CHARACTER.animatedCharacter     = false;
	pf.CHARACTER.backgroundColor       = 0;
	pf.CHARACTER.boldCharacter         = false;
	pf.CHARACTER.capitalCharacter      = false;
	pf.CHARACTER.doublestrikeCharacter = false;
	pf.CHARACTER.embossCharacter       = false;
	pf.CHARACTER.engraveCharacter      = false;
	pf.CHARACTER.expandCharacter       = 0;
	pf.CHARACTER.fontNumber            = 0;
	pf.CHARACTER.fontSize              = 24;
	pf.CHARACTER.foregroundColor       = 0;
	pf.CHARACTER.italicCharacter       = false;
	pf.CHARACTER.kerningCharacter      = 0;
	pf.CHARACTER.outlineCharacter      = false;
	pf.CHARACTER.scaleCharacter        = 100;
	pf.CHARACTER.shadowCharacter       = false;
	pf.CHARACTER.smallcapitalCharacter = false;
	pf.CHARACTER.strikeCharacter       = false;
	pf.CHARACTER.subscriptCharacter    = false;
	pf.CHARACTER.superscriptCharacter  = false;
	pf.CHARACTER.underlineCharacter    = 0;
	pf.NUMS.numsChar  = char(0x95);
	pf.NUMS.numsLevel = 11;
	pf.NUMS.numsSpace = 360;
	pf.SHADING.shadingBkColor   = 0;
	pf.SHADING.shadingFillColor = 0;
	pf.SHADING.shadingIntensity = 0;
	pf.SHADING.shadingType      = RTF_PARAGRAPHSHADINGTYPE_FILL;
	pf.TABS.tabKind     = RTF_PARAGRAPHTABKIND_NONE;
	pf.TABS.tabLead     = RTF_PARAGRAPHTABLEAD_NONE;
	pf.TABS.tabPosition = 0;
	set_paragraphformat(&pf);

	// Set default RTF table row formatting properties
	RTF_TABLEROW_FORMAT rf = {RTF_ROWTEXTALIGN_LEFT, 0, 0, 0, 0, 0, 0};
	set_tablerowformat(&rf);

	// Set default RTF table cell formatting properties
	RTF_TABLECELL_FORMAT cf = {RTF_CELLTEXTALIGN_CENTER, 0, 0, 0, 0, RTF_CELLTEXTDIRECTION_LRTB, false};
	cf.SHADING.shadingBkColor   = 0;
	cf.SHADING.shadingFillColor = 0;
	cf.SHADING.shadingIntensity = 0;
	cf.SHADING.shadingType      = RTF_PARAGRAPHSHADINGTYPE_FILL;
	cf.borderBottom.border              = false;
	cf.borderBottom.BORDERS.borderColor = 0;
	cf.borderBottom.BORDERS.borderKind  = RTF_PARAGRAPHBORDERKIND_NONE;
	cf.borderBottom.BORDERS.borderSpace = 0;
	cf.borderBottom.BORDERS.borderType  = RTF_PARAGRAPHBORDERTYPE_STHICK;
	cf.borderBottom.BORDERS.borderWidth = 5;
	cf.borderLeft  .border              = false;
	cf.borderLeft  .BORDERS.borderColor = 0;
	cf.borderLeft  .BORDERS.borderKind  = RTF_PARAGRAPHBORDERKIND_NONE;
	cf.borderLeft  .BORDERS.borderSpace = 0;
	cf.borderLeft  .BORDERS.borderType  = RTF_PARAGRAPHBORDERTYPE_STHICK;
	cf.borderLeft  .BORDERS.borderWidth = 5;
	cf.borderRight .border              = false;
	cf.borderRight .BORDERS.borderColor = 0;
	cf.borderRight .BORDERS.borderKind  = RTF_PARAGRAPHBORDERKIND_NONE;
	cf.borderRight .BORDERS.borderSpace = 0;
	cf.borderRight .BORDERS.borderType  = RTF_PARAGRAPHBORDERTYPE_STHICK;
	cf.borderRight .BORDERS.borderWidth = 5;
	cf.borderTop   .border              = false;
	cf.borderTop   .BORDERS.borderColor = 0;
	cf.borderTop   .BORDERS.borderKind  = RTF_PARAGRAPHBORDERKIND_NONE;
	cf.borderTop   .BORDERS.borderSpace = 0;
	cf.borderTop   .BORDERS.borderType  = RTF_PARAGRAPHBORDERTYPE_STHICK;
	cf.borderTop   .BORDERS.borderWidth = 5;
	set_tablecellformat(&cf);
}

#if 0
// Sets new RTF document font table
void rtf_writer::set_fonttable( char* fonts )
{
	// Clear old font table
	strcpy( _FontTable, "" );

	// Set separator list
	char separator[] = ";";

	// Create new RTF document font table
	int font_number = 0;
	char font_table_entry[1024];
	char* token = strtok( fonts, separator );
 	while ( token != NULL )
	{
		// Format font table entry
		sprintf( font_table_entry, "{\\f%d\\fnil\\fcharset0\\cpg1252 %s}", font_number, token );
		strcat( _FontTable, font_table_entry );

		// Get next font
		token = strtok( NULL, separator );
		font_number++;
	}
}


// Sets new RTF document color table
void rtf_writer::set_colortable(char* colors)
{
	// Clear old color table
	strcpy( _ColorTable, "" );

	// Set separator list
	char separator[] = ";";

	// Create new RTF document color table
	int color_number = 0;
	char color_table_entry[1024];
	char* token = strtok( colors, separator );
 	while ( token != NULL )
	{
		// Red
		sprintf( color_table_entry, "\\red%s", token );
		strcat( _ColorTable, color_table_entry );

		// Green
		token = strtok( NULL, separator );
		if ( token != NULL )
		{
			sprintf( color_table_entry, "\\green%s", token );
			strcat( _ColorTable, color_table_entry );
		}

		// Blue
		token = strtok( NULL, separator );
		if ( token != NULL )
		{
			sprintf( color_table_entry, "\\blue%s;", token );
			strcat( _ColorTable, color_table_entry );
		}

		// Get next color
		token = strtok( NULL, separator );
		color_number++;
	}
}
#endif


// Sets RTF document formatting properties
void rtf_writer::set_documentformat(RTF_DOCUMENT_FORMAT* df)
{
	// Set new RTF document formatting properties
	memcpy( &_DocFormat, df, sizeof(RTF_DOCUMENT_FORMAT) );
}


// Writes RTF document formatting properties
bool rtf_writer::write_documentformat()
{
	// Set error flag
	bool result = true;

	// RTF document text
	std::ostringstream rtfText;

	rtfText
		<< "\\viewkind"  << _DocFormat.viewKind
		<< "\\viewscale" << _DocFormat.viewScale
		<< "\\paperw"    << _DocFormat.paperWidth
		<< "\\paperh"    << _DocFormat.paperHeight
		<< "\\margl"     << _DocFormat.marginLeft
		<< "\\margr"     << _DocFormat.marginRight
		<< "\\margt"     << _DocFormat.marginTop
		<< "\\margb"     << _DocFormat.marginBottom
		<< "\\gutter"    << _DocFormat.gutterWidth
		;

	if ( _DocFormat.facingPages )
		rtfText << "\\facingp";

	if ( _DocFormat.readOnly )
		rtfText << "\\annotprot";

	// Writes RTF document formatting properties
	fwrite( rtfText.str().c_str(), 1, rtfText.str().size(), _File );

	// Return error flag
	return result;
}


// Sets RTF section formatting properties
void rtf_writer::set_sectionformat(RTF_SECTION_FORMAT* sf)
{
	// Set new RTF section formatting properties
	memcpy( &_SecFormat, sf, sizeof(RTF_SECTION_FORMAT) );
}


// Writes RTF section formatting properties
bool rtf_writer::write_sectionformat()
{
	// Set error flag
	bool result = true;

	// RTF document text
	std::ostringstream rtfText;

	// Format new section
	std::ostringstream text;
	std::ostringstream pgn;

	std::ostringstream sbr;
	std::ostringstream cols;


	if ( _SecFormat.newSection     ) text << "\\sect";
	if ( _SecFormat.defaultSection ) text << "\\sectd";
	if ( _SecFormat.showPageNumber )
	{
		pgn 
			<< "\\pgnx" << _SecFormat.pageNumberOffsetX
			<< "\\pgny" << _SecFormat.pageNumberOffsetY
			;

		text << pgn.str();
	}
	
	// Format section break
	switch (_SecFormat.sectionBreak)
	{
	// Continuous break
	case RTF_SECTIONBREAK_CONTINUOUS:
		sbr << "\\sbknone";
		break;

	// Column break
	case RTF_SECTIONBREAK_COLUMN:
		sbr << "\\sbkcol";
		break;

	// Page break
	case RTF_SECTIONBREAK_PAGE:
		sbr << "\\sbkpage";
		break;

	// Even-page break
	case RTF_SECTIONBREAK_EVENPAGE:
		sbr << "\\sbkeven";
		break;

	// Odd-page break
	case RTF_SECTIONBREAK_ODDPAGE:
		sbr << "\\sbkodd";
		break;
	}

	// Format section columns
	if ( _SecFormat.cols == true )
	{
		// Format columns
		cols 
			<< "\\cols"  << _SecFormat.colsNumber
			<< "\\colsx" << _SecFormat.colsDistance 
			;


		if ( _SecFormat.colsLineBetween )
			cols << "\\linebetcol";
	}

	rtfText
		<< "\n" << text.str() << sbr.str() << cols.str()
		<< "\\pgwsxn"   << _SecFormat.pageWidth       
		<< "\\pghsxn"   << _SecFormat.pageHeight      
		<< "\\marglsxn" << _SecFormat.pageMarginLeft  
		<< "\\margrsxn" << _SecFormat.pageMarginRight 
		<< "\\margtsxn" << _SecFormat.pageMarginTop   
		<< "\\margbsxn" << _SecFormat.pageMarginBottom
		<< "\\guttersxn"<< _SecFormat.pageGutterWidth 
		<< "\\headery"  << _SecFormat.pageHeaderOffset
		<< "\\footery"  << _SecFormat.pageFooterOffset
		;

	// Writes RTF section formatting properties
	fwrite( rtfText.str().c_str(), 1, rtfText.str().size(), _File );

	// Return error flag
	return result;
}


// Starts new RTF section
int rtf_writer::start_section()
{
	// Set error flag
	int error = RTF_SUCCESS;

	// Set new section flag
	_SecFormat.newSection = true;

	// Starts new RTF section
	if( !write_sectionformat() )
		error = RTF_SECTIONFORMAT_ERROR;

	// Return error flag
	return error;
}


// Sets RTF paragraph formatting properties
void rtf_writer::set_paragraphformat(RTF_PARAGRAPH_FORMAT* pf)
{
	// Set new RTF paragraph formatting properties
	memcpy( &_ParFormat, pf, sizeof(RTF_PARAGRAPH_FORMAT) );
}


// Writes RTF paragraph formatting properties
bool rtf_writer::write_paragraphformat()
{
	// Set error flag
	bool result = true;

	// RTF document text
	std::ostringstream rtfText;

	// Format new section
	std::ostringstream text;
	if ( _ParFormat.newParagraph )
		text << "\\par";
	if ( _ParFormat.defaultParagraph )
		text << "\\pard";
	if ( _ParFormat.tableText == false )
		text << "\\plain";
	else
		text << "\\intbl";
	
	switch (_ParFormat.paragraphBreak)
	{
	// No break
	case RTF_PARAGRAPHBREAK_NONE:
		break;

	// Page break;
	case RTF_PARAGRAPHBREAK_PAGE:
		text << "\\page";
		break;

	// Column break;
	case RTF_PARAGRAPHBREAK_COLUMN:
		text << "\\column" ;
		break;

	// Line break;
	case RTF_PARAGRAPHBREAK_LINE:
		text << "\\line";
		break;
	}

	// Format aligment
	switch (_ParFormat.paragraphAligment)
	{
	// Left aligned paragraph
	case RTF_PARAGRAPHALIGN_LEFT:
		text << "\\ql";
		break;

	// Center aligned paragraph
	case RTF_PARAGRAPHALIGN_CENTER:
		text << "\\qc";
		break;

	// Right aligned paragraph
	case RTF_PARAGRAPHALIGN_RIGHT:
		text << "\\qr";
		break;

	// Justified aligned paragraph
	case RTF_PARAGRAPHALIGN_JUSTIFY:
		text << "\\qj";
		break;
	}

	// Format tabs
	if ( _ParFormat.paragraphTabs == true )
	{
		// Set tab kind
		switch ( _ParFormat.TABS.tabKind )
		{
		// No tab
		case RTF_PARAGRAPHTABKIND_NONE:
			break;

		// Centered tab
		case RTF_PARAGRAPHTABKIND_CENTER:
			text << "\\tqc";
			break;

		// Flush-right tab
		case RTF_PARAGRAPHTABKIND_RIGHT:
			text << "\\tqr";
			break;

		// Decimal tab
		case RTF_PARAGRAPHTABKIND_DECIMAL:
			text << "\\tqdec";
			break;
		}

		// Set tab leader
		switch ( _ParFormat.TABS.tabLead )
		{
		// No lead
		case RTF_PARAGRAPHTABLEAD_NONE:
			break;

		// Leader dots
		case RTF_PARAGRAPHTABLEAD_DOT:
			text << "\\tldot";
			break;

		// Leader middle dots
		case RTF_PARAGRAPHTABLEAD_MDOT:
			text << "\\tlmdot";
			break;

		// Leader hyphens
		case RTF_PARAGRAPHTABLEAD_HYPH:
			text << "\\tlhyph";
			break;

		// Leader underline
		case RTF_PARAGRAPHTABLEAD_UNDERLINE:
			text << "\\tlul";
			break;

		// Leader thick line
		case RTF_PARAGRAPHTABLEAD_THICKLINE:
			text << "\\tlth";
			break;

		// Leader equal sign
		case RTF_PARAGRAPHTABLEAD_EQUAL:
			text << "\\tleq";
			break;
		}

		// Set tab position
		text << "\\tx" << _ParFormat.TABS.tabPosition;
	}

	// Format bullets and numbering
	if ( _ParFormat.paragraphNums == true )
	{
		text 
			<< "{\\*\\pn"
			<< "\\pnlvl"   << _ParFormat.NUMS.numsLevel
			<< "\\pnsp"    << _ParFormat.NUMS.numsSpace
			<< "\\pntxtb " << _ParFormat.NUMS.numsChar 
			<< "}"
			;
	}

	// Format paragraph borders
	if ( _ParFormat.paragraphBorders == true )
	{
		std::ostringstream border;

		// Format paragraph border kind
		switch (_ParFormat.BORDERS.borderKind)
		{
		// No border
		case RTF_PARAGRAPHBORDERKIND_NONE:
			break;

		// Border top
		case RTF_PARAGRAPHBORDERKIND_TOP:
			border << "\\brdrt";
			break;

		// Border bottom
		case RTF_PARAGRAPHBORDERKIND_BOTTOM:
			border << "\\brdrb";
			break;

		// Border left
		case RTF_PARAGRAPHBORDERKIND_LEFT:
			border << "\\brdrl";
			break;

		// Border right
		case RTF_PARAGRAPHBORDERKIND_RIGHT:
			border << "\\brdrr";
			break;

		// Border box
		case RTF_PARAGRAPHBORDERKIND_BOX:
			border << "\\box";
			break;
		}

		// Format paragraph border type
		border << get_bordername(_ParFormat.BORDERS.borderType);

		// Set paragraph border width
		border
			<< "\\brdrw" << _ParFormat.BORDERS.borderWidth
			<< "\\brsp"  << _ParFormat.BORDERS.borderSpace
			;

		// Set paragraph border color
		border
			<< "\\brdrcf" << _ParFormat.BORDERS.borderColor
			;


		text << border.str();
	}

	// Format paragraph shading
	if ( _ParFormat.paragraphShading == true )
	{
		// CHECK:
		//std::ostringstream shading;

		//shading << "\\shading" << _ParFormat.SHADING.shadingIntensity;

		// or 

		// Format paragraph shading
		text << get_shadingname( _ParFormat.SHADING.shadingType, false );

		// Set paragraph shading color
		text 
			<< "\\cfpat" << _ParFormat.SHADING.shadingFillColor
			<< "\\cbpat" << _ParFormat.SHADING.shadingBkColor
			;
	}

	// Format paragraph font
	std::ostringstream font;
	font
		<< "\\animtext"  << _ParFormat.CHARACTER.animatedCharacter
		<< "\\expndtw"   << _ParFormat.CHARACTER.expandCharacter  
		<< "\\kerning"   << _ParFormat.CHARACTER.kerningCharacter 
		<< "\\charscalex"<< _ParFormat.CHARACTER.scaleCharacter   
		<< "\\f"         << _ParFormat.CHARACTER.fontNumber       
		<< "\\fs"        << _ParFormat.CHARACTER.fontSize         
		<< "\\cf"        << _ParFormat.CHARACTER.foregroundColor  
		;

	if ( _ParFormat.CHARACTER.boldCharacter )
		font << "\\b";
	else
		font << "\\b0";
	if ( _ParFormat.CHARACTER.capitalCharacter )
		font << "\\caps";
	else
		font << "\\caps0";
	if ( _ParFormat.CHARACTER.doublestrikeCharacter )
		font << "\\striked1";
	else
		font << "\\striked0";
	if ( _ParFormat.CHARACTER.embossCharacter )
		font << "\\embo";
	if ( _ParFormat.CHARACTER.engraveCharacter )
		font << "\\impr";
	if ( _ParFormat.CHARACTER.italicCharacter )
		font << "\\i";
	else
		font << "\\i0";
	if ( _ParFormat.CHARACTER.outlineCharacter )
		font << "\\outl";
	else
		font << "\\outl0";
	if ( _ParFormat.CHARACTER.shadowCharacter )
		font << "\\shad";
	else
		font << "\\shad0";
	if ( _ParFormat.CHARACTER.smallcapitalCharacter )
		font << "\\scaps";
	else
		font << "\\scaps0";
	if ( _ParFormat.CHARACTER.strikeCharacter )
		font << "\\strike";
	else
		font << "\\strike0";
	if ( _ParFormat.CHARACTER.subscriptCharacter )
		font << "\\sub";
	if ( _ParFormat.CHARACTER.superscriptCharacter )
		font << "\\super";
	
	switch (_ParFormat.CHARACTER.underlineCharacter)
	{
	// None underline
	case 0:
		font << "\\ulnone";
		break;

	// Continuous underline
	case 1:
		font << "\\ul";
		break;

	// Dotted underline
	case 2:
		font << "\\uld";
		break;

	// Dashed underline
	case 3:
		font << "\\uldash";
		break;

	// Dash-dotted underline
	case 4:
		font << "\\uldashd";
		break;

	// Dash-dot-dotted underline
	case 5:
		font << "\\uldashdd";
		break;

	// Double underline
	case 6:
		font << "\\uldb";
		break;

	// Heavy wave underline
	case 7:
		font << "\\ulhwave";
		break;

	// Long dashed underline
	case 8:
		font << "\\ulldash";
		break;

	// Thick underline
	case 9:
		font << "\\ulth";
		break;

	// Thick dotted underline
	case 10:
		font << "\\ulthd";
		break;

	// Thick dashed underline
	case 11:
		font << "\\ulthdash";
		break;

	// Thick dash-dotted underline
	case 12:
		font << "\\ulthdashd";
		break;

	// Thick dash-dot-dotted underline
	case 13:
		font << "\\ulthdashdd";
		break;

	// Thick long dashed underline
	case 14:
		font << "\\ulthldash";
		break;

	// Double wave underline
	case 15:
		font << "\\ululdbwave";
		break;

	// Word underline
	case 16:
		font << "\\ulw";
		break;

	// Wave underline
	case 17:
		font << "\\ulwave";
		break;
	}

	// Set paragraph tabbed text
	if ( _ParFormat.tabbedText == false )
	{
		rtfText
			<< "\n"
			<< text.str()
			<< "\\fi" << _ParFormat.firstLineIndent
			<< "\\li" << _ParFormat.leftIndent     
			<< "\\ri" << _ParFormat.rightIndent    
			<< "\\sb" << _ParFormat.spaceBefore    
			<< "\\sa" << _ParFormat.spaceAfter     
			<< "\\sl" << _ParFormat.lineSpacing    
			<< font.str()
			<< " "
			<< _ParFormat.paragraphText;
	}
	else
	{
		rtfText << "\\tab " << _ParFormat.paragraphText;
	}

	// Writes RTF paragraph formatting properties
	fwrite( rtfText.str().c_str(), 1, rtfText.str().size(), _File );

	// Return error flag
	return result;
}


// Starts new RTF paragraph
int rtf_writer::start_paragraph(char* text, bool newPar)
{
	// Set error flag
	int error = RTF_SUCCESS;

	// Copy paragraph text
	_ParFormat.paragraphText = new char[strlen(text)];
	strcpy( _ParFormat.paragraphText, text );

	// Set new paragraph
	_ParFormat.newParagraph = newPar;

	// Starts new RTF paragraph
	if( !write_paragraphformat() )
		error = RTF_PARAGRAPHFORMAT_ERROR;

	// Return error flag
	return error;
}


// Gets RTF document formatting properties
RTF_DOCUMENT_FORMAT* rtf_writer::get_documentformat()
{
	// Get current RTF document formatting properties
	return &_DocFormat;
}


// Gets RTF section formatting properties
RTF_SECTION_FORMAT* rtf_writer::get_sectionformat()
{
	// Get current RTF section formatting properties
	return &_SecFormat;
}


// Gets RTF paragraph formatting properties
RTF_PARAGRAPH_FORMAT* rtf_writer::get_paragraphformat()
{
	// Get current RTF paragraph formatting properties
	return &_ParFormat;
}


// Loads image from file
int rtf_writer::load_image(char* image, int width, int height)
{
	// Set error flag
	int error = RTF_SUCCESS;

	// Check image type
	bool err = false;
	if ( strstr(image,".bmp") == NULL )
	{
		if ( strstr(image,".jpg") == NULL )
		{
			if (strstr(image,".gif") == NULL )
				err = true;
		}
	}

	// If valid image type
	if ( err == false )
	{
		// Free IPicture object
		if ( _Picture != NULL )
		{
			_Picture->Release();
			_Picture = NULL;
		}

		// Read image file
		int imageFile = _open( image, _O_RDONLY | _O_BINARY );
		struct _stat st;
		_fstat( imageFile, &st );
		DWORD nSize = st.st_size;
		BYTE* pBuff = new BYTE[nSize];
		_read( imageFile, pBuff, nSize );
		// Alocate memory for image data
		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, nSize);
		void* pData = GlobalLock(hGlobal);
		memcpy(pData, pBuff, nSize);
		GlobalUnlock(hGlobal);
		// Load image using OLE
		IStream* pStream = NULL;
		if ( CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) == S_OK )
		{
			HRESULT hr;
			if ((hr = OleLoadPicture( pStream, nSize, FALSE, IID_IPicture, (LPVOID *)&_Picture)) != S_OK)
				error = RTF_IMAGE_ERROR;

			pStream->Release();
		}
		delete []pBuff;
		_close(imageFile);

		// If image is loaded
		if ( _Picture != NULL )
		{
			// Calculate image size
			long hmWidth;
			long hmHeight;
			_Picture->get_Width(&hmWidth);
			_Picture->get_Height(&hmHeight);
			int nWidth	= MulDiv( hmWidth, GetDeviceCaps(GetDC(NULL),LOGPIXELSX), 2540 );
			int nHeight	= MulDiv( hmHeight, GetDeviceCaps(GetDC(NULL),LOGPIXELSY), 2540 );

			// Create metafile;
			HDC hdcMeta = CreateMetaFile(NULL);

			// Render picture to metafile
			_Picture->Render( hdcMeta, 0, 0, nWidth, nHeight, 0, hmHeight, hmWidth, -hmHeight, NULL );

			// Close metafile
			HMETAFILE hmf = CloseMetaFile(hdcMeta);

			// Get metafile data
			UINT size = GetMetaFileBitsEx( hmf, 0, NULL );
			BYTE* buffer = new BYTE[size];
			GetMetaFileBitsEx( hmf, size, buffer );
			DeleteMetaFile(hmf);

			// Convert metafile binary data to hexadecimal
			char* str = bin_hex_convert( buffer, size );
			delete []buffer;

			// Format picture paragraph
			RTF_PARAGRAPH_FORMAT* pf = get_paragraphformat();
			pf->paragraphText = "";
			write_paragraphformat();

			// Writes RTF picture data
			char rtfText[1024];
			sprintf( rtfText, "\n{\\pict\\wmetafile8\\picwgoal%d\\pichgoal%d\\picscalex%d\\picscaley%d\n", hmWidth, hmHeight, width, height );
			if ( fwrite( rtfText, 1, strlen(rtfText), _File ) < strlen(rtfText) )
				error = RTF_IMAGE_ERROR;
			fwrite( str, 1, 2*size, _File );
			strcpy( rtfText, "}" );
			fwrite( rtfText, 1, strlen(rtfText), _File );
		}
	}
	else
	{
		// Writes RTF picture data
		char rtfText[1024];
		strcpy( rtfText, "\n\\par\\pard *** Error! Wrong image format ***\\par" );
		fwrite( rtfText, 1, strlen(rtfText), _File );
	}

	// Return error flag
	return error;
}


// Converts binary data to hex
char* rtf_writer::bin_hex_convert(unsigned char* binary, int size)
{
	char* result = new char[2*size];

	char part1, part2;
	for ( int i=0; i<size; i++ )
	{
		part1 = binary[i] / 16;
		if ( part1 < 10 )
			part1 += 48;
		else
		{
			if ( part1 == 10 )
				part1 = 'a';
			if ( part1 == 11 )
				part1 = 'b';
			if ( part1 == 12 )
				part1 = 'c';
			if ( part1 == 13 )
				part1 = 'd';
			if ( part1 == 14 )
				part1 = 'e';
			if ( part1 == 15 )
				part1 = 'f';
		}

		part2 = binary[i] % 16;
		if ( part2 < 10 )
			part2 += 48;
		else
		{
			if ( part2 == 10 )
				part2 = 'a';
			if ( part2 == 11 )
				part2 = 'b';
			if ( part2 == 12 )
				part2 = 'c';
			if ( part2 == 13 )
				part2 = 'd';
			if ( part2 == 14 )
				part2 = 'e';
			if ( part2 == 15 )
				part2 = 'f';
		}

		result[2*i] = part1;
		result[2*i+1] = part2;
	}

	strcat( result, "\0" );

	return result;
}


// Starts new RTF table row
int rtf_writer::start_tablerow()
{
	// Set error flag
	int error = RTF_SUCCESS;

	char tblrw[1024] = "";
	// Format table row aligment
	switch (_RowFormat.rowAligment)
	{
		// Left align
		case RTF_ROWTEXTALIGN_LEFT:
			strcat( tblrw, "\\trql" );
			break;

		// Center align
		case RTF_ROWTEXTALIGN_CENTER:
			strcat( tblrw, "\\trqc" );
			break;

		// Right align
		case RTF_ROWTEXTALIGN_RIGHT:
			strcat( tblrw, "\\trqr" );
			break;
	}

	// Writes RTF table data
	char rtfText[1024];
	sprintf( rtfText, "\n\\trowd\\trgaph115%s\\trleft%d\\trrh%d\\trpaddb%d\\trpaddfb3\\trpaddl%d\\trpaddfl3\\trpaddr%d\\trpaddfr3\\trpaddt%d\\trpaddft3", 
		tblrw, _RowFormat.rowLeftMargin, _RowFormat.rowHeight, _RowFormat.marginTop, _RowFormat.marginBottom, _RowFormat.marginLeft, _RowFormat.marginRight );
	if ( fwrite( rtfText, 1, strlen(rtfText), _File ) < strlen(rtfText) )
		error = RTF_TABLE_ERROR;

	// Return error flag
	return error;
}


// Ends RTF table row
int rtf_writer::end_tablerow()
{
	// Set error flag
	int error = RTF_SUCCESS;

	// Writes RTF table data
	char rtfText[1024];
	sprintf( rtfText, "\n\\trgaph115\\row\\pard" );
	if ( fwrite( rtfText, 1, strlen(rtfText), _File ) < strlen(rtfText) )
		error = RTF_TABLE_ERROR;

	// Return error flag
	return error;
}


// Starts new RTF table cell
int rtf_writer::start_tablecell(int rightMargin)
{
	// Set error flag
	int error = RTF_SUCCESS;

	char tblcla[20];
	// Format table cell text aligment
	switch (_CellFormat.textVerticalAligment)
	{
		// Top align
		case RTF_CELLTEXTALIGN_TOP:
			strcpy( tblcla, "\\clvertalt" );
			break;

		// Center align
		case RTF_CELLTEXTALIGN_CENTER:
			strcpy( tblcla, "\\clvertalc" );
			break;

		// Bottom align
		case RTF_CELLTEXTALIGN_BOTTOM:
			strcpy( tblcla, "\\clvertalb" );
			break;
	}

	char tblcld[20];
	// Format table cell text direction
	switch (_CellFormat.textDirection)
	{
		// Left to right, top to bottom
		case RTF_CELLTEXTDIRECTION_LRTB:
			strcpy( tblcld, "\\cltxlrtb" );
			break;

		// Right to left, top to bottom
		case RTF_CELLTEXTDIRECTION_RLTB:
			strcpy( tblcld, "\\cltxtbrl" );
			break;

		// Left to right, bottom to top
		case RTF_CELLTEXTDIRECTION_LRBT:
			strcpy( tblcld, "\\cltxbtlr" );
			break;

		// Left to right, top to bottom, vertical
		case RTF_CELLTEXTDIRECTION_LRTBV:
			strcpy( tblcld, "\\cltxlrtbv" );
			break;

		// Right to left, top to bottom, vertical
		case RTF_CELLTEXTDIRECTION_RLTBV:
			strcpy( tblcld, "\\cltxtbrlv" );
			break;
	}

	char tbclbrb[1024]="", tbclbrl[1024]="", tbclbrr[1024]="", tbclbrt[1024]="";
	// Format table cell border
	if ( _CellFormat.borderBottom.border == true )
	{
		// Bottom cell border
		char tbclbt[20];
		strcpy( tbclbt, "\\clbrdrb" );

		char* border = get_bordername(_CellFormat.borderBottom.BORDERS.borderType);

		sprintf( tbclbrb, "%s%s\\brdrw%d\\brsp%d\\brdrcf%d", tbclbt, border, _CellFormat.borderBottom.BORDERS.borderWidth, 
			_CellFormat.borderBottom.BORDERS.borderSpace, _CellFormat.borderBottom.BORDERS.borderColor );
	}
	if ( _CellFormat.borderLeft.border == true )
	{
		// Left cell border
		char tbclbt[20];
		strcpy( tbclbt, "\\clbrdrl" );

		char* border = get_bordername(_CellFormat.borderLeft.BORDERS.borderType);

		sprintf( tbclbrl, "%s%s\\brdrw%d\\brsp%d\\brdrcf%d", tbclbt, border, _CellFormat.borderLeft.BORDERS.borderWidth, 
		_CellFormat.borderLeft.BORDERS.borderSpace, _CellFormat.borderLeft.BORDERS.borderColor );
	}
	if ( _CellFormat.borderRight.border == true )
	{
		// Right cell border
		char tbclbt[20];
		strcpy( tbclbt, "\\clbrdrr" );

		char* border = get_bordername(_CellFormat.borderRight.BORDERS.borderType);

		sprintf( tbclbrr, "%s%s\\brdrw%d\\brsp%d\\brdrcf%d", tbclbt, border, _CellFormat.borderRight.BORDERS.borderWidth, 
		_CellFormat.borderRight.BORDERS.borderSpace, _CellFormat.borderRight.BORDERS.borderColor );
	}
	if ( _CellFormat.borderTop.border == true )
	{
		// Top cell border
		char tbclbt[20];
		strcpy( tbclbt, "\\clbrdrt" );

		char* border = get_bordername(_CellFormat.borderTop.BORDERS.borderType);

		sprintf( tbclbrt, "%s%s\\brdrw%d\\brsp%d\\brdrcf%d", tbclbt, border, _CellFormat.borderTop.BORDERS.borderWidth, 
		_CellFormat.borderTop.BORDERS.borderSpace, _CellFormat.borderTop.BORDERS.borderColor );
	}

	// Format table cell shading
	char shading[100] = "";
	if ( _CellFormat.cellShading == true )
	{
		char* sh = get_shadingname( _CellFormat.SHADING.shadingType, true );

		// Set paragraph shading color
		sprintf( shading, "%s\\clshdgn%d\\clcfpat%d\\clcbpat%d", sh, _CellFormat.SHADING.shadingIntensity, _CellFormat.SHADING.shadingFillColor, _CellFormat.SHADING.shadingBkColor );
	}

	// Writes RTF table data
	char rtfText[1024];
	sprintf( rtfText, "\n\\tcelld%s%s%s%s%s%s%s\\cellx%d", tblcla, tblcld, tbclbrb, tbclbrl, tbclbrr, tbclbrt, shading, rightMargin );
	if ( fwrite( rtfText, 1, strlen(rtfText), _File ) < strlen(rtfText) )
		error = RTF_TABLE_ERROR;

	// Return error flag
	return error;
}


// Ends RTF table cell
int rtf_writer::end_tablecell()
{
	// Set error flag
	int error = RTF_SUCCESS;

	// Writes RTF table data
	std::ostringstream rtfText;

	rtfText << "\n\\cell ";
	fwrite( rtfText.str().c_str(), 1, rtfText.str().size(), _File );


	// Return error flag
	return error;
}


// Gets RTF table row formatting properties
RTF_TABLEROW_FORMAT* rtf_writer::get_tablerowformat()
{
	// Get current RTF table row formatting properties
	return &_RowFormat;
}


// Sets RTF table row formatting properties
void rtf_writer::set_tablerowformat(RTF_TABLEROW_FORMAT* rf)
{
	// Set new RTF table row formatting properties
	memcpy( &_RowFormat, rf, sizeof(RTF_TABLEROW_FORMAT) );
}


// Gets RTF table cell formatting properties
RTF_TABLECELL_FORMAT* rtf_writer::get_tablecellformat()
{
	// Get current RTF table cell formatting properties
	return &_CellFormat;
}


// Sets RTF table cell formatting properties
void rtf_writer::set_tablecellformat(RTF_TABLECELL_FORMAT* cf)
{
	// Set new RTF table cell formatting properties
	memcpy( &_CellFormat, cf, sizeof(RTF_TABLECELL_FORMAT) );
}


// Gets border name
char* rtf_writer::get_bordername(int border_type)
{
	char* border = new char[20];

	switch (border_type)
	{
		// Single-thickness border
		case RTF_PARAGRAPHBORDERTYPE_STHICK:
			strcpy( border, "\\brdrs" );
			break;

		// Double-thickness border
		case RTF_PARAGRAPHBORDERTYPE_DTHICK:
			strcpy( border, "\\brdrth" );
			break;

		// Shadowed border
		case RTF_PARAGRAPHBORDERTYPE_SHADOW:
			strcpy( border, "\\brdrsh" );
			break;

		// Double border
		case RTF_PARAGRAPHBORDERTYPE_DOUBLE:
			strcpy( border, "\\brdrdb" );
			break;

		// Dotted border
		case RTF_PARAGRAPHBORDERTYPE_DOT:
			strcpy( border, "\\brdrdot" );
			break;

		// Dashed border
		case RTF_PARAGRAPHBORDERTYPE_DASH:
			strcpy( border, "\\brdrdash" );
			break;

		// Hairline border
		case RTF_PARAGRAPHBORDERTYPE_HAIRLINE:
			strcpy( border, "\\brdrhair" );
			break;

		// Inset border
		case RTF_PARAGRAPHBORDERTYPE_INSET:
			strcpy( border, "\\brdrinset" );
			break;

		// Dashed border (small)
		case RTF_PARAGRAPHBORDERTYPE_SDASH:
			strcpy( border, "\\brdrdashsm" );
			break;

		// Dot-dashed border
		case RTF_PARAGRAPHBORDERTYPE_DOTDASH:
			strcpy( border, "\\brdrdashd" );
			break;

		// Dot-dot-dashed border
		case RTF_PARAGRAPHBORDERTYPE_DOTDOTDASH:
			strcpy( border, "\\brdrdashdd" );
			break;

		// Outset border
		case RTF_PARAGRAPHBORDERTYPE_OUTSET:
			strcpy( border, "\\brdroutset" );
			break;

		// Triple border
		case RTF_PARAGRAPHBORDERTYPE_TRIPLE:
			strcpy( border, "\\brdrtriple" );
			break;

		// Wavy border
		case RTF_PARAGRAPHBORDERTYPE_WAVY:
			strcpy( border, "\\brdrwavy" );
			break;

		// Double wavy border
		case RTF_PARAGRAPHBORDERTYPE_DWAVY:
			strcpy( border, "\\brdrwavydb" );
			break;

		// Striped border
		case RTF_PARAGRAPHBORDERTYPE_STRIPED:
			strcpy( border, "\\brdrdashdotstr" );
			break;

		// Embossed border
		case RTF_PARAGRAPHBORDERTYPE_EMBOSS:
			strcpy( border, "\\brdremboss" );
			break;

		// Engraved border
		case RTF_PARAGRAPHBORDERTYPE_ENGRAVE:
			strcpy( border, "\\brdrengrave" );
			break;
	}

	return border;
}


// Gets shading name
char* rtf_writer::get_shadingname(int shading_type, bool cell)
{
	char* shading = new char[20];

	if ( cell == false )
	{
		switch (shading_type)
		{
			// Fill shading
			case RTF_PARAGRAPHSHADINGTYPE_FILL:
				strcpy( shading, "" );
				break;

			// Horizontal background pattern
			case RTF_PARAGRAPHSHADINGTYPE_HORIZ:
				strcpy( shading, "\\bghoriz" );
				break;

			// Vertical background pattern
			case RTF_PARAGRAPHSHADINGTYPE_VERT:
				strcpy( shading, "\\bgvert" );
				break;

			// Forward diagonal background pattern
			case RTF_PARAGRAPHSHADINGTYPE_FDIAG:
				strcpy( shading, "\\bgfdiag" );
				break;

			// Backward diagonal background pattern
			case RTF_PARAGRAPHSHADINGTYPE_BDIAG:
				strcpy( shading, "\\bgbdiag" );
				break;

			// Cross background pattern
			case RTF_PARAGRAPHSHADINGTYPE_CROSS:
				strcpy( shading, "\\bgcross" );
				break;

			// Diagonal cross background pattern
			case RTF_PARAGRAPHSHADINGTYPE_CROSSD:
				strcpy( shading, "\\bgdcross" );
				break;

			// Dark horizontal background pattern
			case RTF_PARAGRAPHSHADINGTYPE_DHORIZ:
				strcpy( shading, "\\bgdkhoriz" );
				break;

			// Dark vertical background pattern
			case RTF_PARAGRAPHSHADINGTYPE_DVERT:
				strcpy( shading, "\\bgdkvert" );
				break;

			// Dark forward diagonal background pattern
			case RTF_PARAGRAPHSHADINGTYPE_DFDIAG:
				strcpy( shading, "\\bgdkfdiag" );
				break;

			// Dark backward diagonal background pattern
			case RTF_PARAGRAPHSHADINGTYPE_DBDIAG:
				strcpy( shading, "\\bgdkbdiag" );
				break;

			// Dark cross background pattern
			case RTF_PARAGRAPHSHADINGTYPE_DCROSS:
				strcpy( shading, "\\bgdkcross" );
				break;

			// Dark diagonal cross background pattern
			case RTF_PARAGRAPHSHADINGTYPE_DCROSSD:
				strcpy( shading, "\\bgdkdcross" );
				break;
		}
	}
	else
	{
		switch (shading_type)
		{
			// Fill shading
			case RTF_CELLSHADINGTYPE_FILL:
				strcpy( shading, "" );
				break;

			// Horizontal background pattern
			case RTF_CELLSHADINGTYPE_HORIZ:
				strcpy( shading, "\\clbghoriz" );
				break;

			// Vertical background pattern
			case RTF_CELLSHADINGTYPE_VERT:
				strcpy( shading, "\\clbgvert" );
				break;

			// Forward diagonal background pattern
			case RTF_CELLSHADINGTYPE_FDIAG:
				strcpy( shading, "\\clbgfdiag" );
				break;

			// Backward diagonal background pattern
			case RTF_CELLSHADINGTYPE_BDIAG:
				strcpy( shading, "\\clbgbdiag" );
				break;

			// Cross background pattern
			case RTF_CELLSHADINGTYPE_CROSS:
				strcpy( shading, "\\clbgcross" );
				break;

			// Diagonal cross background pattern
			case RTF_CELLSHADINGTYPE_CROSSD:
				strcpy( shading, "\\clbgdcross" );
				break;

			// Dark horizontal background pattern
			case RTF_CELLSHADINGTYPE_DHORIZ:
				strcpy( shading, "\\clbgdkhoriz" );
				break;

			// Dark vertical background pattern
			case RTF_CELLSHADINGTYPE_DVERT:
				strcpy( shading, "\\clbgdkvert" );
				break;

			// Dark forward diagonal background pattern
			case RTF_CELLSHADINGTYPE_DFDIAG:
				strcpy( shading, "\\clbgdkfdiag" );
				break;

			// Dark backward diagonal background pattern
			case RTF_CELLSHADINGTYPE_DBDIAG:
				strcpy( shading, "\\clbgdkbdiag" );
				break;

			// Dark cross background pattern
			case RTF_CELLSHADINGTYPE_DCROSS:
				strcpy( shading, "\\clbgdkcross" );
				break;

			// Dark diagonal cross background pattern
			case RTF_CELLSHADINGTYPE_DCROSSD:
				strcpy( shading, "\\clbgdkdcross" );
				break;
		}
	}

	return shading;
}
