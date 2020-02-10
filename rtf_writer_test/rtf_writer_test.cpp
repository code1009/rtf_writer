#include "stdafx.h"

#include "rtf_writer.hpp"

void rtf_writer_test (void)
{
	rtf_writer rtf;

	RTF_PARAGRAPH_FORMAT* pf = rtf.get_paragraphformat();
	RTF_SECTION_FORMAT*   sf = rtf.get_sectionformat  ();
	RTF_TABLEROW_FORMAT*  rf = rtf.get_tablerowformat ();
	RTF_TABLECELL_FORMAT* cf = rtf.get_tablecellformat();


	// Set RTF document font and color table
	rtf.initialize();

	rtf.reset_fonttable();
	rtf.reset_colortable();
	rtf.add_font("굴림", 0, 129);
	rtf.add_color(0,0,0);
	rtf.add_color(255,255,255);
	rtf.add_color(255,0,0);
	rtf.add_color(0,255,0);
	rtf.add_color(128,128,255);
	// Open RTF
	rtf.open();

	//-----------------------------------------------------------------------
#if 0
	rtf.start_section();

	rf->rowAligment = RTF_ROWTEXTALIGN_CENTER;
	rf->marginTop    =120;
	rf->marginBottom =120;
	rf->marginLeft   =120;
	rf->marginRight  =120;

	rtf.start_tablerow();
	{
		// Format table cell
		cf->textVerticalAligment = RTF_CELLTEXTALIGN_CENTER;
		cf->textDirection        = RTF_CELLTEXTDIRECTION_LRTB;
		cf->borderBottom.border              = true;
		cf->borderBottom.BORDERS.borderType  = RTF_PARAGRAPHBORDERTYPE_STHICK;
		cf->borderBottom.BORDERS.borderWidth = 5;
		cf->borderLeft  .border              = true;
		cf->borderLeft  .BORDERS.borderType  = RTF_PARAGRAPHBORDERTYPE_STHICK;
		cf->borderLeft  .BORDERS.borderWidth = 5;
		cf->borderRight .border              = true;
		cf->borderRight .BORDERS.borderType  = RTF_PARAGRAPHBORDERTYPE_STHICK;
		cf->borderRight .BORDERS.borderWidth = 5;
		cf->borderTop   .border              = true;
		cf->borderTop   .BORDERS.borderType  = RTF_PARAGRAPHBORDERTYPE_STHICK;
		cf->borderTop   .BORDERS.borderWidth = 5;
		cf->cellShading              = true;
		cf->SHADING.shadingType      = RTF_CELLSHADINGTYPE_FILL;
		cf->SHADING.shadingBkColor   = 4;
		cf->SHADING.shadingFillColor = 3;

		pf->tableText = true;
//		pf->paragraphAligment = RTF_PARAGRAPHALIGN_JUSTIFY;
		pf->CHARACTER.foregroundColor = 1;
		pf->CHARACTER.fontSize = 20;

		rtf.start_tablecell(2000);
		rtf.start_tablecell(4000);
		rtf.start_tablecell(6000);
			rtf.start_paragraph( "한글1a", false );
			rtf.start_paragraph( "한글a", true );
		rtf.end_tablecell();
			pf->CHARACTER.foregroundColor = 0;
			rtf.start_paragraph( "한글2a", false );
			rtf.start_paragraph( "한글a", true );
		rtf.end_tablecell();
			rtf.start_paragraph( "한글2a", false );
			rtf.start_paragraph( "한글a", true );
		rtf.end_tablecell();
	}
	rtf.end_tablerow();
	pf->tableText = false;
	rtf.start_paragraph( "", true );
	
	sf->sectionBreak = RTF_SECTIONBREAK_PAGE;
	rtf.start_section();
	sf->sectionBreak = RTF_SECTIONBREAK_CONTINUOUS;
	rtf.start_paragraph( "", true );
	rtf.start_paragraph( "한글3a", false );
	rtf.start_paragraph( "First section:", true );


#endif

#if 1
	//-----------------------------------------------------------------------
	// Write paragraph text
	rtf.start_paragraph( "First section:", false );

	//-----------------------------------------------------------------------
	// Format paragraph
	pf->paragraphTabs    = true;
	pf->TABS.tabKind     = RTF_PARAGRAPHTABKIND_RIGHT;
	pf->TABS.tabLead     = RTF_PARAGRAPHTABLEAD_DOT;
	pf->TABS.tabPosition = 7200;
	// Write paragraph text
	rtf.start_paragraph( "This is new paragraph text", true );
	// Write paragraph tabbed text
	pf->paragraphTabs = false;
	pf->tabbedText    = true;
	rtf.start_paragraph( "Some tabbed text", false );

	//-----------------------------------------------------------------------
	pf->tabbedText = false;
	// Format paragraph
	pf->firstLineIndent = 360;
	// Write paragraph text
	pf->paragraphTabs = true;
	rtf.start_paragraph( "This is another paragraph text", true );
	// Write paragraph tabbed text
	pf->paragraphTabs = false;
	pf->tabbedText = true;
	rtf.start_paragraph( "Some more tabbed text", false );
	pf->tabbedText = false;

	//-----------------------------------------------------------------------
	// Format paragraph
	pf->firstLineIndent = 0;
	pf->CHARACTER.italicCharacter = true;

	// Write paragraph text
	rtf.start_paragraph( "This is new paragraph text", true );

	//-----------------------------------------------------------------------
	// Create new section
	rtf.start_section();
	// Format paragraph
	pf->spaceBefore = 120;
	pf->paragraphAligment = RTF_PARAGRAPHALIGN_CENTER;
	pf->CHARACTER.italicCharacter = false;
	pf->CHARACTER.foregroundColor = 1;
	// Write paragraph text
	rtf.start_paragraph( "Second section:", false );
	// Format paragraph
	pf->spaceBefore = 0;
	pf->CHARACTER.foregroundColor = 0;
	// Write paragraph text
	rtf.start_paragraph( "Plain paragraph text", true );

	//-----------------------------------------------------------------------
	// Create new section
	rtf.start_section();
	// Format paragraph
	pf->spaceBefore = 120;
	pf->paragraphAligment = RTF_PARAGRAPHALIGN_RIGHT;
	pf->CHARACTER.underlineCharacter = 17;
	// Write paragraph text
	rtf.start_paragraph( "Third section:", false );
	// Format paragraph
	pf->spaceBefore = 0;
	pf->CHARACTER.boldCharacter = true;
	pf->CHARACTER.subscriptCharacter = true;
	pf->CHARACTER.underlineCharacter = 0;
	// Write paragraph text
	rtf.start_paragraph( "Plain paragraph text", true );

	//-----------------------------------------------------------------------
	// Create new section
	rtf.start_section();
	// Format paragraph
	pf->spaceBefore = 120;
	pf->paragraphAligment = RTF_PARAGRAPHALIGN_LEFT;
	pf->paragraphBorders  = true;
	pf->BORDERS.borderKind  = RTF_PARAGRAPHBORDERKIND_BOX;
	pf->BORDERS.borderType  = RTF_PARAGRAPHBORDERTYPE_ENGRAVE;
	pf->BORDERS.borderWidth = 0;
	pf->BORDERS.borderColor = 0;
	pf->BORDERS.borderSpace = 120;
	pf->CHARACTER.boldCharacter        = false;
	pf->CHARACTER.subscriptCharacter   = false;
	pf->CHARACTER.superscriptCharacter = true;
	// Write paragraph text
	rtf.start_paragraph( "Fourth section:", false );

	//-----------------------------------------------------------------------
	// Create new section
	rtf.start_section();
	// Format paragraph
	pf->spaceBefore = 120;
	pf->paragraphBorders  = false;
	pf->paragraphAligment = RTF_PARAGRAPHALIGN_LEFT;
	pf->paragraphShading  = true;
	pf->SHADING.shadingIntensity = 0;
	pf->SHADING.shadingType      = RTF_PARAGRAPHSHADINGTYPE_FILL;
	pf->SHADING.shadingFillColor = 0;
	pf->SHADING.shadingBkColor   = 2;
	pf->CHARACTER.fontNumber           = 1;
	pf->CHARACTER.fontSize             = 20;
	pf->CHARACTER.superscriptCharacter = false;
	// Write paragraph text
	rtf.start_paragraph( "Fifth section:", false );
	// Format paragraph
	pf->paragraphShading = false;
	// Write paragraph text (empty paragraphs)
	rtf.start_paragraph( "", true );
	rtf.start_paragraph( "", true );

	//-----------------------------------------------------------------------
	// Format section
	sf->cols            = true;
	sf->colsDistance    = 720;
	sf->colsLineBetween = true;
	sf->colsNumber      = 2;
	// Create new section
	rtf.start_section();
	// Format paragraph
	pf->paragraphShading = false;
	pf->spaceBefore = 0;
	// Write paragraph text
	rtf.start_paragraph( "Column text is here as an example of what this library can do...", false );
	// Format paragraph
	pf->paragraphBreak = RTF_PARAGRAPHBREAK_COLUMN;
	// Write paragraph text
	rtf.start_paragraph( "Also, it can be very powerful tool in right hands...", false );

	//-----------------------------------------------------------------------
	// Format section
	sf->cols = false;
	// Create new section
	rtf.start_section();
	// Format paragraph
	pf->paragraphAligment = RTF_PARAGRAPHALIGN_CENTER;
	pf->paragraphBreak    = 0;
	pf->spaceBefore = 360;
	pf->spaceAfter  = 360;
	// Load image (*.bmp, *.jpg, *.gif)
	rtf.load_image("Picture.jpg", 50, 50);

	//-----------------------------------------------------------------------
	// Format section
	sf->cols            = true;
	sf->colsDistance    = 360;
	sf->colsLineBetween = false;
	sf->colsNumber      = 2;
	// Create new section
	rtf.start_section();
	// Format paragraph
	pf->paragraphNums     = true;
	pf->paragraphAligment = RTF_PARAGRAPHALIGN_CENTER;
	pf->spaceBefore = 0;
	pf->spaceAfter  = 0;
	pf->NUMS.numsLevel = 11;
	pf->NUMS.numsSpace = 360;
//	pf->NUMS.numsChar  = char(0x95);
	// Write paragraph text (bulleted)
	rtf.start_paragraph( "Bulleted text1", false );
	rtf.start_paragraph( "Bulleted text2", true );
	rtf.start_paragraph( "Bulleted text3", true );
	rtf.start_paragraph( "Bulleted text4", true );
	rtf.start_paragraph( "Bulleted text5", true );

	// Format paragraph
	pf->paragraphBreak = 2;
	// Write paragraph text
	rtf.start_paragraph( "", true );
	// Format paragraph
	pf->paragraphBreak = 0;
	pf->NUMS.numsLevel = 3;
	rtf.start_paragraph( "Numbered text1", false );
	rtf.start_paragraph( "Numbered text2", true );
	rtf.start_paragraph( "Numbered text3", true );
	rtf.start_paragraph( "Numbered text4", true );
	rtf.start_paragraph( "Numbered text5", true );
	// Format paragraph
	pf->paragraphBreak = 0;
	pf->paragraphNums  = false;
	// Write paragraph text
	rtf.start_paragraph( "", true );
	// Format paragraph
	pf->spaceBefore = 360;
	pf->spaceAfter  = 360;
	// Write paragraph text
	rtf.start_paragraph( " ", true );

	//-----------------------------------------------------------------------
	// Format section
	sf->cols = false;
	// Create new section
	rtf.start_section();
	// Format paragraph
	pf->spaceBefore = 0;
	pf->spaceAfter  = 0;

	// Format table row
	rf->rowAligment = RTF_ROWTEXTALIGN_CENTER;
	rf->marginTop    = 120;
	rf->marginBottom = 120;
	rf->marginLeft   = 120;
	rf->marginRight  = 120;

	// Start table row
	rtf.start_tablerow();
	{
		// Format table cell
		cf->textVerticalAligment = RTF_CELLTEXTALIGN_CENTER;
		cf->textDirection        = RTF_CELLTEXTDIRECTION_LRTB;
		cf->borderBottom.border              = true;
		cf->borderBottom.BORDERS.borderType  = RTF_PARAGRAPHBORDERTYPE_STHICK;
		cf->borderBottom.BORDERS.borderWidth = 5;
		cf->borderLeft  .border              = true;
		cf->borderLeft  .BORDERS.borderType  = RTF_PARAGRAPHBORDERTYPE_STHICK;
		cf->borderLeft  .BORDERS.borderWidth = 5;
		cf->borderRight .border              = true;
		cf->borderRight .BORDERS.borderType  = RTF_PARAGRAPHBORDERTYPE_STHICK;
		cf->borderRight .BORDERS.borderWidth = 30;
		cf->borderTop   .border              = true;
		cf->borderTop   .BORDERS.borderType  = RTF_PARAGRAPHBORDERTYPE_STHICK;
		cf->borderTop   .BORDERS.borderWidth = 5;
		cf->cellShading            = true;
		cf->SHADING.shadingType    = RTF_CELLSHADINGTYPE_FILL;
		cf->SHADING.shadingBkColor = 3;
		
		// Start table cell
		rtf.start_tablecell(2000);
		{
			// Format table cell
			cf->borderLeft .BORDERS.borderWidth = 30;
			cf->borderRight.BORDERS.borderWidth = 5;

			// Start table cell
			cf->SHADING.shadingBkColor = 2;
			rtf.start_tablecell(4000);
			{
				// Format paragraph
				pf->tableText = true;
				pf->paragraphAligment = RTF_PARAGRAPHALIGN_JUSTIFY;

				// Write paragraph text
				rtf.start_paragraph( "This is table cell text...", false );
				rtf.start_paragraph( "These paragraphs are enclosed in table cell", true );
			}
			// End table cell
			rtf.end_tablecell();
			// Write paragraph text
			pf->CHARACTER.boldCharacter = true;
			rtf.start_paragraph( "This text is in another cell...", false );
			rtf.start_paragraph( "You must define correct cell right margin in order to see the text", true );
			pf->CHARACTER.boldCharacter = false;
		}
		// End table cell
		rtf.end_tablecell();
	}
	// End table row
	rtf.end_tablerow();

	// Start table row
	rtf.start_tablerow();
	{
		// Format table cell
		cf->borderLeft.BORDERS.borderWidth = 5;
		cf->SHADING.shadingBkColor = 3;

		// Start table cell
		rtf.start_tablecell(4000);
		{
			cf->cellShading = false;
			// Write paragraph text
			rtf.start_paragraph( "This text is in another row, and these cells are merged...", false );
			pf->tableText = false;
		}
		// End table cell
		rtf.end_tablecell();
	}

	// End table row
	rtf.end_tablerow();
#endif

	//-----------------------------------------------------------------------
	// Close RTF
	rtf.close();

	{
		FILE* _File = fopen( "d:\\Sample.rtf", "w" );
		fwrite( rtf._oss.str().c_str(), 1, rtf._oss.str().size(), _File );
		fclose(_File);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	std::ostringstream oss;
	std::string a,b,c;
	char d = 'a';

	a="a";
	b="";
	oss << a << b << a << "{" << d << "}" << "\r\n";

	printf ("%s", oss.str().c_str());


	rtf_writer_test();


	return 0;
}

