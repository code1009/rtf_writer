#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <io.h>
#include <olectl.h>

#include "rtf_writer.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
rtf_writer::rtf_writer()
{
	_Codepage     = RTF_DEFAULT_CODEPAGE    ;
	_LanguageID   = RTF_DEFAULT_LANGUAGEID  ;
	_LanguageIDFe = RTF_DEFAULT_LANGUAGEIDFE;

	_FontCount = 0;
}

rtf_writer::~rtf_writer()
{
}

std::string rtf_writer::to_escape_string (std::string l)
{
	std::string h;

	unsigned char ch;
	unsigned int i;
	unsigned int count;

	char part1;
	char part2;


	// \'b0\'a1
	count = l.size();
	h.reserve(count*2);
	for (i=0u; i<count; i++)
	{
		ch = l.at(i);

		if (ch<128)
		{
			h.push_back(ch);
		}
		else
		{
			h.push_back('\\');
			h.push_back('\'');

			part1 =  ch / 16;
			if ( part1 < 10 )
			{
				part1 = '0' + part1;
			}
			else
			{
				part1 = 'a' + (part1-10);
			}

			part2 = ch % 16;
			if ( part2 < 10 )
			{
				part2 = '0' + part2;
			}
			else
			{
				part2 = 'a' + (part2-10);
			}

			h.push_back(part1);
			h.push_back(part2);
		}
	}

	return h;
}

void rtf_writer::initialize()
{
	// Initialize global params
	set_defaultfont();
	set_defaultcolor();
	set_defaultformat();
}

// Open RTF document
void rtf_writer::open()
{
	std::string s;

	s.reserve(1024*1024);
	_oss.str(s);


	// Write RTF document header
	write_header();

	// Write RTF document formatting properties
	write_documentformat();

	// Create first RTF document section with default formatting
	write_sectionformat();
}


// Closes RTF document
void rtf_writer::close()
{
	// Write RTF document end part
	std::ostringstream rtfText;

	rtfText << "\n\\par}";

	_oss << rtfText.str();
}


// Writes RTF document header
void rtf_writer::write_header()
{
	// Standard RTF document header
	std::ostringstream rtfText;
	rtfText 
		<< "{"
			<< "\\rtf1"
			<< "\\ansi"
			<< "\\ansicpg" << _Codepage
			<< "\\deff0"
			;
	  
	if (0!=_LanguageIDFe)
	{
		rtfText 
			<< "\\deflang"   << _LanguageID
			<< "\\deflangfe" << _LanguageIDFe
			;
	}

	if (!_FontTable.empty())
	{
		rtfText 
			<< "{"
				<< "\\fonttbl" << _FontTable
			<< "}"
			;
	}

	if (!_ColorTable.empty())
	{
		rtfText 
			<< "{"
				<< "\\colortbl" << _ColorTable
			<< "}"
			;
	}

	rtfText 
			<< "{" 
				<< "\\*\\generator " << "rtf_writer;"
			<< "}" << "\n"
			<< "{"
				<< "\\info"
				<< "{\\author "  << "code1009"  << "}"
				<< "{\\company " << "SEMI Company LTD." << "}"
			<< "}"
			;

	// Writes standard RTF document header part
	_oss << rtfText.str();
}

void rtf_writer::set_defaultfont()
{
	std::ostringstream rtfText;

	// Set RTF document default font table
	rtfText 
		<< "{\\f0\\froman\\fcharset0\\cpg1252 Times New Roman}"
		<< "{\\f1\\fswiss\\fcharset0\\cpg1252 Arial}"
		<< "{\\f2\\fmodern\\fcharset0\\cpg1252 Courier New}"
		<< "{\\f3\\fscript\\fcharset0\\cpg1252 Cursive}"
		<< "{\\f4\\fdecor\\fcharset0\\cpg1252 Old English}"
		<< "{\\f5\\ftech\\fcharset0\\cpg1252 Symbol}"
		<< "{\\f6\\fbidi\\fcharset0\\cpg1252 Miriam}"
		;
	
	_FontTable = rtfText.str();
	_FontCount = 7;
}

void rtf_writer::set_defaultcolor()
{
	std::ostringstream rtfText;

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
}

void rtf_writer::reset_fonttable()
{
	_FontTable.clear();
	_FontCount = 0;
}

void rtf_writer::reset_colortable()
{
	_ColorTable.clear();
}

void rtf_writer::add_font(char* facename, int codepage, int charset)
{
//  {\f0\fnil\fcharset0\cpg1252 Times New Roman}
//	{\f0\fnil\fcharset129 \'b8\'bc\'c0\'ba \'b0\'ed\'b5\'f1;}

	std::ostringstream rtfText;

	
	rtfText
		<< "{"
		<< "\\f"   << _FontCount
		<< "\\fnil"
		;

	{
		rtfText
		<< "\\fcharset" << charset
		;
	}

	if (0!=codepage)
	{
		rtfText
		<< "\\cpg" << codepage
		;
	}

	rtfText
		<< " "
		<< to_escape_string(facename)
		<< "}"
		;

	_FontTable += rtfText.str();



	_FontCount++;
}

void rtf_writer::add_color(int r, int g, int b)
{
	std::ostringstream rtfText;

	
	rtfText
		<< "\\red"   << r
		<< "\\green" << g
		<< "\\blue"  << b
		<< ";"
		;

	_ColorTable += rtfText.str();
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
	RTF_PARAGRAPH_FORMAT pf = {RTF_PARAGRAPHBREAK_NONE, false, true, RTF_PARAGRAPHALIGN_LEFT, 0, 0, 0, 0, 0, 0, /*NULL,*/ false, false, false, false, false, false};
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
//	pf.NUMS.numsChar  = char(0xb7);
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


// Sets RTF document formatting properties
void rtf_writer::set_documentformat(RTF_DOCUMENT_FORMAT* df)
{
	// Set new RTF document formatting properties
	memcpy( &_DocFormat, df, sizeof(RTF_DOCUMENT_FORMAT) );
}


// Writes RTF document formatting properties
void rtf_writer::write_documentformat()
{
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
	_oss << rtfText.str();
}


// Sets RTF section formatting properties
void rtf_writer::set_sectionformat(RTF_SECTION_FORMAT* sf)
{
	// Set new RTF section formatting properties
	memcpy( &_SecFormat, sf, sizeof(RTF_SECTION_FORMAT) );
}


// Writes RTF section formatting properties
void rtf_writer::write_sectionformat()
{
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
	_oss << rtfText.str();
}


// Starts new RTF section
void rtf_writer::start_section()
{
	// Set new section flag
	_SecFormat.newSection = true;

	// Starts new RTF section
	write_sectionformat();
}


// Sets RTF paragraph formatting properties
void rtf_writer::set_paragraphformat(RTF_PARAGRAPH_FORMAT* pf)
{
	// Set new RTF paragraph formatting properties
	memcpy( &_ParFormat, pf, sizeof(RTF_PARAGRAPH_FORMAT) );
}


// Writes RTF paragraph formatting properties
void rtf_writer::write_paragraphformat()
{
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
			<< "\\pntxtb " << to_escape_string(std::string (&_ParFormat.NUMS.numsChar,1))
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
			<< to_escape_string(_ParagraphText);
	}
	else
	{
		rtfText << "\\tab " << to_escape_string(_ParagraphText);
	}

	// Writes RTF paragraph formatting properties
	_oss << rtfText.str();
}


// Starts new RTF paragraph
void rtf_writer::start_paragraph(char* text, bool newPar)
{
	// Copy paragraph text
	_ParagraphText = text;
//	_ParFormat.paragraphText = _ParagraphText.c_str();

	// Set new paragraph
	_ParFormat.newParagraph = newPar;

	// Starts new RTF paragraph
	write_paragraphformat();
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

static HGLOBAL file_to_hglobal (char* file_path)
{
	HGLOBAL hglobal;

	int            file_handle;
	struct _stat   file_stat;
	unsigned int   file_size;
	void*          file_contents;


	hglobal = NULL;


	file_handle = _open(file_path, _O_RDONLY | _O_BINARY);
	if (-1!=file_handle)
	{
		_fstat(file_handle, &file_stat);
		file_size = file_stat.st_size;


		hglobal = GlobalAlloc(GMEM_MOVEABLE, file_size);
		if (hglobal)
		{
			file_contents = GlobalLock(hglobal);

			_read(file_handle, file_contents, file_size);

			GlobalUnlock(hglobal);
		}

		_close(file_handle);
	}

	return hglobal;
}

static IPicture* image_file_to_ipicture (char* image_file_path)
{
	IPicture* result;
	IPicture* ipicture;
	IStream*  istream;
	HGLOBAL   hglobal;
	HRESULT   hr;

	unsigned int size;


	result   = NULL;
	ipicture = NULL;
	istream  = NULL;
	hglobal  = file_to_hglobal(image_file_path);
	if (hglobal)
	{
		size = GlobalSize(hglobal);

		hr = CreateStreamOnHGlobal(hglobal, TRUE, &istream);
		if (S_OK==hr)
		{
			hr = OleLoadPicture(istream, size, FALSE, IID_IPicture, (LPVOID*)&ipicture);
			if (S_OK==hr)
			{
				result = ipicture;
			}

			istream->Release();
		}

		GlobalFree(hglobal);
	}


	return result;
}

static HMETAFILE ipicture_to_hmetafile (IPicture* ipicture)
{
	// Calculate image size
	long hmWidth ;
	long hmHeight;
	int  nWidth  ;
	int  nHeight ;
	
	HDC hdcMeta;

	HMETAFILE hmf;


	ipicture->get_Width (&hmWidth);
	ipicture->get_Height(&hmHeight);

	nWidth	= MulDiv( hmWidth , GetDeviceCaps(GetDC(NULL),LOGPIXELSX), 2540 );
	nHeight	= MulDiv( hmHeight, GetDeviceCaps(GetDC(NULL),LOGPIXELSY), 2540 );

	// Create metafile;
	hdcMeta = CreateMetaFile(NULL);

	// Render picture to metafile
	ipicture->Render( hdcMeta, 0, 0, nWidth, nHeight, 0, hmHeight, hmWidth, -hmHeight, NULL );

	// Close metafile
	hmf = CloseMetaFile(hdcMeta);


	return hmf;
}

typedef struct _image_hmetafile_data_t
{
	long width;
	long height;
	std::vector<unsigned char> binary;
}
image_hmetafile_data_t;

static void get_image_hmetafile_data (char* image_file_path, image_hmetafile_data_t& image_hmetafile_data)
{
	image_hmetafile_data.width  = 0;
	image_hmetafile_data.height = 0;
	image_hmetafile_data.binary.clear();
	

	IPicture* ipicture;
	HMETAFILE hmf;

	unsigned int   size;
	unsigned char* pointer;


	ipicture = image_file_to_ipicture(image_file_path);
	if (ipicture)
	{
		ipicture->get_Width (&image_hmetafile_data.width );
		ipicture->get_Height(&image_hmetafile_data.height);

		hmf = ipicture_to_hmetafile(ipicture);
		if (hmf)
		{
			size = GetMetaFileBitsEx( hmf, 0, NULL );

			image_hmetafile_data.binary.assign(size, 0);
			pointer = & *image_hmetafile_data.binary.begin();

			GetMetaFileBitsEx( hmf, size, pointer );

			DeleteMetaFile(hmf);
		}

		ipicture->Release();
	}
}

// Loads image from file
void rtf_writer::load_image(char* image_file_path, int width, int height)
{
	// RTF document text
	std::ostringstream rtfText;


	image_hmetafile_data_t image_hmetafile_data;


	get_image_hmetafile_data(image_file_path, image_hmetafile_data);
	if ( image_hmetafile_data.binary.empty() )
	{
		rtfText << "\n\\par\\pard " << "ERROR:" << image_file_path << "\\par";

		_oss << rtfText.str();

		return;
	}


	unsigned int   size;
	unsigned char* pointer;
	std::string    hex;

	pointer = & *image_hmetafile_data.binary.begin();
	size    =    image_hmetafile_data.binary.size();

	hex = binary_to_hex( pointer, size );

	// Format picture paragraph
	RTF_PARAGRAPH_FORMAT* pf = get_paragraphformat();
//	pf->paragraphText = NULL;
	_ParagraphText.clear();
	write_paragraphformat();

	// Writes RTF picture data
	rtfText
		<< "\n"
		<< "{"
		<< "\\pict\\wmetafile8"
		<< "\\picwgoal"  << image_hmetafile_data.width 
		<< "\\pichgoal"  << image_hmetafile_data.height
		<< "\\picscalex" << width
		<< "\\picscaley" << height
		<< "\n"
		<< hex
		<< "}"
		;

	_oss << rtfText.str();
}


// Converts binary data to hex
std::string rtf_writer::binary_to_hex(unsigned char* binary, int size)
{
	std::string str;
	str.reserve(size*2);

	std::ostringstream hex(str);

	unsigned char ch;
	char part1;
	char part2;
	int  i;

	for (i=0; i<size; i++ )
	{
		ch = binary[i];

		part1 =  ch / 16;
		if ( part1 < 10 )
		{
			part1 = '0' + part1;
		}
		else
		{
			part1 = 'a' + (part1-10);
		}

		part2 = ch % 16;
		if ( part2 < 10 )
		{
			part2 = '0' + part2;
		}
		else
		{
			part2 = 'a' + (part2-10);
		}

		hex << part1 << part2;
	}

	return hex.str();
}


// Starts new RTF table row
void rtf_writer::start_tablerow()
{
	std::ostringstream rtfText;


	// Format table row aligment
	std::ostringstream text;
	switch (_RowFormat.rowAligment)
	{
	// Left align
	case RTF_ROWTEXTALIGN_LEFT:
		text << "\\trql";
		break;

	// Center align
	case RTF_ROWTEXTALIGN_CENTER:
		text << "\\trqc";
		break;

	// Right align
	case RTF_ROWTEXTALIGN_RIGHT:
		text << "\\trqr";
		break;
	}

	// Writes RTF table data
	rtfText
		<< "\n"
		<< "\\trowd"
		<< "\\trgaph115" << text.str()
		<< "\\trleft"    << _RowFormat.rowLeftMargin
		<< "\\trrh"      << _RowFormat.rowHeight
		<< "\\trpaddb"   << _RowFormat.marginTop
		<< "\\trpaddfb3"
		<< "\\trpaddl"   << _RowFormat.marginBottom
		<< "\\trpaddfl3"
		<< "\\trpaddr"   << _RowFormat.marginLeft
		<< "\\trpaddfr3"
		<< "\\trpaddt"   << _RowFormat.marginRight
		<< "\\trpaddft3";

	_oss << rtfText.str();
}


// Ends RTF table row
void rtf_writer::end_tablerow()
{
	std::ostringstream rtfText;


	// Writes RTF table data
	rtfText
		<< "\n"
		<< "\\trgaph115"
		<< "\\row"
		<< "\\pard";

	_oss << rtfText.str();
}


// Starts new RTF table cell
void rtf_writer::start_tablecell(int rightMargin)
{
	std::ostringstream rtfText;

	// Format table cell text aligment
	std::ostringstream text;
	switch (_CellFormat.textVerticalAligment)
	{
	// Top align
	case RTF_CELLTEXTALIGN_TOP:
		text << "\\clvertalt";
		break;

	// Center align
	case RTF_CELLTEXTALIGN_CENTER:
		text << "\\clvertalc";
		break;

	// Bottom align
	case RTF_CELLTEXTALIGN_BOTTOM:
		text << "\\clvertalb";
		break;
	}

	// Format table cell text direction
	switch (_CellFormat.textDirection)
	{
	// Left to right, top to bottom
	case RTF_CELLTEXTDIRECTION_LRTB:
		text << "\\cltxlrtb";
		break;

	// Right to left, top to bottom
	case RTF_CELLTEXTDIRECTION_RLTB:
		text << "\\cltxtbrl";
		break;

	// Left to right, bottom to top
	case RTF_CELLTEXTDIRECTION_LRBT:
		text << "\\cltxbtlr";
		break;

	// Left to right, top to bottom, vertical
	case RTF_CELLTEXTDIRECTION_LRTBV:
		text << "\\cltxlrtbv";
		break;

	// Right to left, top to bottom, vertical
	case RTF_CELLTEXTDIRECTION_RLTBV:
		text << "\\cltxtbrlv";
		break;
	}

	// Format table cell border
	std::ostringstream border;
	if ( _CellFormat.borderBottom.border == true )
	{
		// Bottom cell border
		border
			<< "\\clbrdrb"
			<< get_bordername(_CellFormat.borderBottom.BORDERS.borderType)
			<< "\\brdrw"  << _CellFormat.borderBottom.BORDERS.borderWidth
			<< "\\brsp"   << _CellFormat.borderBottom.BORDERS.borderSpace
			<< "\\brdrcf" << _CellFormat.borderBottom.BORDERS.borderColor
			;
	}
	if ( _CellFormat.borderLeft.border == true )
	{
		// Left cell border
		border
			<< "\\clbrdrl"
			<< get_bordername(_CellFormat.borderLeft.BORDERS.borderType)
			<< "\\brdrw"  << _CellFormat.borderLeft.BORDERS.borderWidth
			<< "\\brsp"   << _CellFormat.borderLeft.BORDERS.borderSpace
			<< "\\brdrcf" << _CellFormat.borderLeft.BORDERS.borderColor
			;
	}
	if ( _CellFormat.borderRight.border == true )
	{
		// Right cell border
		border
			<< "\\clbrdrr"
			<< get_bordername(_CellFormat.borderRight.BORDERS.borderType)
			<< "\\brdrw"  << _CellFormat.borderRight.BORDERS.borderWidth
			<< "\\brsp"   << _CellFormat.borderRight.BORDERS.borderSpace
			<< "\\brdrcf" << _CellFormat.borderRight.BORDERS.borderColor
			;
	}
	if ( _CellFormat.borderTop.border == true )
	{
		// Top cell border
		border
			<< "\\clbrdrt"
			<< get_bordername(_CellFormat.borderTop.BORDERS.borderType)
			<< "\\brdrw"  << _CellFormat.borderTop.BORDERS.borderWidth
			<< "\\brsp"   << _CellFormat.borderTop.BORDERS.borderSpace
			<< "\\brdrcf" << _CellFormat.borderTop.BORDERS.borderColor
			;
	}

	// Format table cell shading
	std::ostringstream shading;
	if ( _CellFormat.cellShading == true )
	{
		// Set paragraph shading color
		shading
			<< get_shadingname(_CellFormat.SHADING.shadingType, true)
			<< "\\clshdgn" << _CellFormat.SHADING.shadingIntensity
			<< "\\clcfpat" << _CellFormat.SHADING.shadingFillColor
			<< "\\clcbpat" << _CellFormat.SHADING.shadingBkColor
			;
	}

	// Writes RTF table data
	rtfText
		<< "\n"
		<< "\\tcelld"
		<< text.str()
		<< border.str()
		<< shading.str()
		<< "\\cellx" << rightMargin;

	_oss << rtfText.str();
}


// Ends RTF table cell
void rtf_writer::end_tablecell()
{
	// Writes RTF table data
	std::ostringstream rtfText;

	rtfText << "\n\\cell ";
	_oss << rtfText.str();
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
std::string rtf_writer::get_bordername(int border_type)
{
	std::string border;

	switch (border_type)
	{
	// Single-thickness border
	case RTF_PARAGRAPHBORDERTYPE_STHICK:
		border ="\\brdrs";
		break;

	// Double-thickness border
	case RTF_PARAGRAPHBORDERTYPE_DTHICK:
		border ="\\brdrth";
		break;

	// Shadowed border
	case RTF_PARAGRAPHBORDERTYPE_SHADOW:
		border ="\\brdrsh";
		break;

	// Double border
	case RTF_PARAGRAPHBORDERTYPE_DOUBLE:
		border ="\\brdrdb";
		break;

	// Dotted border
	case RTF_PARAGRAPHBORDERTYPE_DOT:
		border ="\\brdrdot";
		break;

	// Dashed border
	case RTF_PARAGRAPHBORDERTYPE_DASH:
		border ="\\brdrdash";
		break;

	// Hairline border
	case RTF_PARAGRAPHBORDERTYPE_HAIRLINE:
		border ="\\brdrhair";
		break;

	// Inset border
	case RTF_PARAGRAPHBORDERTYPE_INSET:
		border ="\\brdrinset";
		break;

	// Dashed border (small)
	case RTF_PARAGRAPHBORDERTYPE_SDASH:
		border ="\\brdrdashsm";
		break;

	// Dot-dashed border
	case RTF_PARAGRAPHBORDERTYPE_DOTDASH:
		border ="\\brdrdashd";
		break;

	// Dot-dot-dashed border
	case RTF_PARAGRAPHBORDERTYPE_DOTDOTDASH:
		border ="\\brdrdashdd";
		break;

	// Outset border
	case RTF_PARAGRAPHBORDERTYPE_OUTSET:
		border ="\\brdroutset";
		break;

	// Triple border
	case RTF_PARAGRAPHBORDERTYPE_TRIPLE:
		border ="\\brdrtriple";
		break;

	// Wavy border
	case RTF_PARAGRAPHBORDERTYPE_WAVY:
		border ="\\brdrwavy";
		break;

	// Double wavy border
	case RTF_PARAGRAPHBORDERTYPE_DWAVY:
		border ="\\brdrwavydb";
		break;

	// Striped border
	case RTF_PARAGRAPHBORDERTYPE_STRIPED:
		border ="\\brdrdashdotstr";
		break;

	// Embossed border
	case RTF_PARAGRAPHBORDERTYPE_EMBOSS:
		border ="\\brdremboss";
		break;

	// Engraved border
	case RTF_PARAGRAPHBORDERTYPE_ENGRAVE:
		border ="\\brdrengrave";
		break;
	}

	return border;
}


// Gets shading name
std::string rtf_writer::get_shadingname(int shading_type, bool cell)
{
	std::string shading;

	if ( cell == false )
	{
		switch (shading_type)
		{
		// Fill shading
		case RTF_PARAGRAPHSHADINGTYPE_FILL:
			shading = "";
			break;

		// Horizontal background pattern
		case RTF_PARAGRAPHSHADINGTYPE_HORIZ:
			shading = "\\bghoriz";
			break;

		// Vertical background pattern
		case RTF_PARAGRAPHSHADINGTYPE_VERT:
			shading = "\\bgvert";
			break;

		// Forward diagonal background pattern
		case RTF_PARAGRAPHSHADINGTYPE_FDIAG:
			shading = "\\bgfdiag";
			break;

		// Backward diagonal background pattern
		case RTF_PARAGRAPHSHADINGTYPE_BDIAG:
			shading = "\\bgbdiag";
			break;

		// Cross background pattern
		case RTF_PARAGRAPHSHADINGTYPE_CROSS:
			shading = "\\bgcross";
			break;

		// Diagonal cross background pattern
		case RTF_PARAGRAPHSHADINGTYPE_CROSSD:
			shading = "\\bgdcross";
			break;

		// Dark horizontal background pattern
		case RTF_PARAGRAPHSHADINGTYPE_DHORIZ:
			shading = "\\bgdkhoriz";
			break;

		// Dark vertical background pattern
		case RTF_PARAGRAPHSHADINGTYPE_DVERT:
			shading = "\\bgdkvert";
			break;

		// Dark forward diagonal background pattern
		case RTF_PARAGRAPHSHADINGTYPE_DFDIAG:
			shading = "\\bgdkfdiag";
			break;

		// Dark backward diagonal background pattern
		case RTF_PARAGRAPHSHADINGTYPE_DBDIAG:
			shading = "\\bgdkbdiag";
			break;

		// Dark cross background pattern
		case RTF_PARAGRAPHSHADINGTYPE_DCROSS:
			shading = "\\bgdkcross";
			break;

		// Dark diagonal cross background pattern
		case RTF_PARAGRAPHSHADINGTYPE_DCROSSD:
			shading = "\\bgdkdcross";
			break;
		}
	}
	else
	{
		switch (shading_type)
		{
		// Fill shading
		case RTF_CELLSHADINGTYPE_FILL:
			shading = "";
			break;

		// Horizontal background pattern
		case RTF_CELLSHADINGTYPE_HORIZ:
			shading = "\\clbghoriz";
			break;

		// Vertical background pattern
		case RTF_CELLSHADINGTYPE_VERT:
			shading = "\\clbgvert";
			break;

		// Forward diagonal background pattern
		case RTF_CELLSHADINGTYPE_FDIAG:
			shading = "\\clbgfdiag";
			break;

		// Backward diagonal background pattern
		case RTF_CELLSHADINGTYPE_BDIAG:
			shading = "\\clbgbdiag";
			break;

		// Cross background pattern
		case RTF_CELLSHADINGTYPE_CROSS:
			shading = "\\clbgcross";
			break;

		// Diagonal cross background pattern
		case RTF_CELLSHADINGTYPE_CROSSD:
			shading = "\\clbgdcross";
			break;

		// Dark horizontal background pattern
		case RTF_CELLSHADINGTYPE_DHORIZ:
			shading = "\\clbgdkhoriz";
			break;

		// Dark vertical background pattern
		case RTF_CELLSHADINGTYPE_DVERT:
			shading = "\\clbgdkvert";
			break;

		// Dark forward diagonal background pattern
		case RTF_CELLSHADINGTYPE_DFDIAG:
			shading = "\\clbgdkfdiag";
			break;

		// Dark backward diagonal background pattern
		case RTF_CELLSHADINGTYPE_DBDIAG:
			shading = "\\clbgdkbdiag";
			break;

		// Dark cross background pattern
		case RTF_CELLSHADINGTYPE_DCROSS:
			shading = "\\clbgdkcross";
			break;

		// Dark diagonal cross background pattern
		case RTF_CELLSHADINGTYPE_DCROSSD:
			shading = "\\clbgdkdcross";
			break;
		}
	}

	return shading;
}
