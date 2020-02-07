// rtf_writer_test.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include "rtf_writer.hpp"

int _tmain(int argc, _TCHAR* argv[])
{
	std::ostringstream oss;
	std::string a,b,c;

	a="a";
	b="";
	oss << a << b << a;
	printf ("%s", oss.str().c_str());

	// http://www.biblioscape.com/rtf15_spec.htm

	rtf_writer _rtf;


	// Set RTF document font and color table
	char font_list[] = "Times New Roman;Arial;";
	char color_list[] = "0;0;0;255;0;0;192;192;192;255;255;255";
	
	// Open RTF file
	_rtf.open( "d:\\Sample.rtf", font_list, color_list );

	// Write paragraph text
	_rtf.start_paragraph( "First section:", false );
	// Format paragraph
	RTF_PARAGRAPH_FORMAT* pf = _rtf.get_paragraphformat();
	pf->paragraphTabs = true;
	pf->TABS.tabKind = RTF_PARAGRAPHTABKIND_RIGHT;
	pf->TABS.tabLead = RTF_PARAGRAPHTABLEAD_DOT;
	pf->TABS.tabPosition = 7200;
	// Write paragraph text
	_rtf.start_paragraph( "This is new paragraph text", true );
	// Write paragraph tabbed text
	pf->paragraphTabs = false;
	pf->tabbedText = true;
	_rtf.start_paragraph( "Some tabbed text", false );
	pf->tabbedText = false;
	// Format paragraph
	pf->firstLineIndent = 360;
	// Write paragraph text
	pf->paragraphTabs = true;
	_rtf.start_paragraph( "This is another paragraph text", true );
	// Write paragraph tabbed text
	pf->paragraphTabs = false;
	pf->tabbedText = true;
	_rtf.start_paragraph( "Some more tabbed text", false );
	pf->tabbedText = false;
	// Format paragraph
	pf->firstLineIndent = 0;
	pf->CHARACTER.italicCharacter = true;
	// Write paragraph text
	_rtf.start_paragraph( "This is new paragraph text", true );

	// Create new section
	_rtf.start_section();
	// Format paragraph
	pf->spaceBefore = 120;
	pf->paragraphAligment = RTF_PARAGRAPHALIGN_CENTER;
	pf->CHARACTER.italicCharacter = false;
	pf->CHARACTER.foregroundColor = 1;
	// Write paragraph text
	_rtf.start_paragraph( "Second section:", false );
	// Format paragraph
	pf->spaceBefore = 0;
	pf->CHARACTER.foregroundColor = 0;
	// Write paragraph text
	_rtf.start_paragraph( "Plain paragraph text", true );

	// Create new section
	_rtf.start_section();
	// Format paragraph
	pf->spaceBefore = 120;
	pf->paragraphAligment = RTF_PARAGRAPHALIGN_RIGHT;
	pf->CHARACTER.underlineCharacter = 17;
	// Write paragraph text
	_rtf.start_paragraph( "Third section:", false );
	// Format paragraph
	pf->spaceBefore = 0;
	pf->CHARACTER.boldCharacter = true;
	pf->CHARACTER.subscriptCharacter = true;
	pf->CHARACTER.underlineCharacter = 0;
	// Write paragraph text
	_rtf.start_paragraph( "Plain paragraph text", true );

	// Create new section
	_rtf.start_section();
	// Format paragraph
	pf->spaceBefore = 120;
	pf->paragraphAligment = RTF_PARAGRAPHALIGN_LEFT;
	pf->paragraphBorders = true;
	pf->BORDERS.borderKind = RTF_PARAGRAPHBORDERKIND_BOX;
	pf->BORDERS.borderType = RTF_PARAGRAPHBORDERTYPE_ENGRAVE;
	pf->BORDERS.borderWidth = 0;
	pf->BORDERS.borderColor = 0;
	pf->BORDERS.borderSpace = 120;
	pf->CHARACTER.boldCharacter = false;
	pf->CHARACTER.subscriptCharacter = false;
	pf->CHARACTER.superscriptCharacter = true;
	// Write paragraph text
	_rtf.start_paragraph( "Fourth section:", false );

	// Create new section
	_rtf.start_section();
	// Format paragraph
	pf->spaceBefore = 120;
	pf->paragraphBorders = false;
	pf->paragraphAligment = RTF_PARAGRAPHALIGN_LEFT;
	pf->paragraphShading = true;
	pf->SHADING.shadingIntensity = 0;
	pf->SHADING.shadingType = RTF_PARAGRAPHSHADINGTYPE_FILL;
	pf->SHADING.shadingFillColor = 0;
	pf->SHADING.shadingBkColor = 2;
	pf->CHARACTER.fontNumber = 1;
	pf->CHARACTER.fontSize = 20;
	pf->CHARACTER.superscriptCharacter = false;
	// Write paragraph text
	_rtf.start_paragraph( "Fifth section:", false );
	// Format paragraph
	pf->paragraphShading = false;
	// Write paragraph text (empty paragraphs)
	_rtf.start_paragraph( "", true );
	_rtf.start_paragraph( "", true );

	// Format section
	RTF_SECTION_FORMAT* sf = _rtf.get_sectionformat();
	sf->cols = true;
	sf->colsDistance = 720;
	sf->colsLineBetween = true;
	sf->colsNumber = 2;
	// Create new section
	_rtf.start_section();
	// Format paragraph
	pf->paragraphShading = false;
	pf->spaceBefore = 0;
	// Write paragraph text
	_rtf.start_paragraph( "Column text is here as an example of what this library can do...", false );
	// Format paragraph
	pf->paragraphBreak = RTF_PARAGRAPHBREAK_COLUMN;
	// Write paragraph text
	_rtf.start_paragraph( "Also, it can be very powerful tool in right hands...", false );

	// Format section
	sf->cols = false;
	// Create new section
	_rtf.start_section();
	// Format paragraph
	pf->paragraphAligment = RTF_PARAGRAPHALIGN_CENTER;
	pf->paragraphBreak = 0;
	pf->spaceBefore = 360;
	pf->spaceAfter = 360;
	// Load image (*.bmp, *.jpg, *.gif)
	_rtf.load_image("Picture.jpg", 50, 50);

	// Format section
	sf->cols = true;
	sf->colsDistance = 360;
	sf->colsLineBetween = false;
	sf->colsNumber = 2;
	// Create new section
	_rtf.start_section();
	// Format paragraph
	pf->paragraphNums = true;
	pf->paragraphAligment = RTF_PARAGRAPHALIGN_CENTER;
	pf->spaceBefore = 0;
	pf->spaceAfter = 0;
	pf->NUMS.numsLevel = 11;
	pf->NUMS.numsSpace = 360;
	pf->NUMS.numsChar = char(0x95);
	// Write paragraph text (bulleted)
	_rtf.start_paragraph( "Bulleted text1", false );
	_rtf.start_paragraph( "Bulleted text2", true );
	_rtf.start_paragraph( "Bulleted text3", true );
	_rtf.start_paragraph( "Bulleted text4", true );
	_rtf.start_paragraph( "Bulleted text5", true );
	// Format paragraph
	pf->paragraphBreak = 2;
	// Write paragraph text
	_rtf.start_paragraph( "", true );
	// Format paragraph
	pf->paragraphBreak = 0;
	pf->NUMS.numsLevel = 3;
	_rtf.start_paragraph( "Numbered text1", false );
	_rtf.start_paragraph( "Numbered text2", true );
	_rtf.start_paragraph( "Numbered text3", true );
	_rtf.start_paragraph( "Numbered text4", true );
	_rtf.start_paragraph( "Numbered text5", true );
	// Format paragraph
	pf->paragraphBreak = 0;
	pf->paragraphNums = false;
	// Write paragraph text
	_rtf.start_paragraph( "", true );
	// Format paragraph
	pf->spaceBefore = 360;
	pf->spaceAfter = 360;
	// Write paragraph text
	_rtf.start_paragraph( " ", true );

	// Format section
	sf->cols = false;
	// Create new section
	_rtf.start_section();
	// Format paragraph
	pf->spaceBefore = 0;
	pf->spaceAfter = 0;
	// Format table row
	RTF_TABLEROW_FORMAT* rf = _rtf.get_tablerowformat();
	rf->rowAligment = RTF_ROWTEXTALIGN_CENTER;
	rf->marginTop = 120;
	rf->marginBottom = 120;
	rf->marginLeft = 120;
	rf->marginRight = 120;
	// Start table row
	_rtf.start_tablerow();
	// Format table cell
	RTF_TABLECELL_FORMAT* cf = _rtf.get_tablecellformat();
	cf->textVerticalAligment = RTF_CELLTEXTALIGN_CENTER;
	cf->textDirection = RTF_CELLTEXTDIRECTION_LRTB;
	cf->borderBottom.border = true;
	cf->borderBottom.BORDERS.borderType = RTF_PARAGRAPHBORDERTYPE_STHICK;
	cf->borderBottom.BORDERS.borderWidth = 5;
	cf->borderLeft.border = true;
	cf->borderLeft.BORDERS.borderType = RTF_PARAGRAPHBORDERTYPE_STHICK;
	cf->borderLeft.BORDERS.borderWidth = 5;
	cf->borderRight.border = true;
	cf->borderRight.BORDERS.borderType = RTF_PARAGRAPHBORDERTYPE_STHICK;
	cf->borderRight.BORDERS.borderWidth = 30;
	cf->borderTop.border = true;
	cf->borderTop.BORDERS.borderType = RTF_PARAGRAPHBORDERTYPE_STHICK;
	cf->borderTop.BORDERS.borderWidth = 5;
	cf->cellShading = true;
	cf->SHADING.shadingType = RTF_CELLSHADINGTYPE_FILL;
	cf->SHADING.shadingBkColor = 3;
	// Start table cell
	_rtf.start_tablecell(2000);
	// Format table cell
	cf->borderLeft.BORDERS.borderWidth = 30;
	cf->borderRight.BORDERS.borderWidth = 5;
	// Start table cell
	cf->SHADING.shadingBkColor = 2;
	_rtf.start_tablecell(4000);
	// Format paragraph
	pf->tableText = true;
	pf->paragraphAligment = RTF_PARAGRAPHALIGN_JUSTIFY;
	// Write paragraph text
	_rtf.start_paragraph( "This is table cell text...", false );
	_rtf.start_paragraph( "These paragraphs are enclosed in table cell", true );
	// End table cell
	_rtf.end_tablecell();
	// Write paragraph text
	pf->CHARACTER.boldCharacter = true;
	_rtf.start_paragraph( "This text is in another cell...", false );
	_rtf.start_paragraph( "You must define correct cell right margin in order to see the text", true );
	pf->CHARACTER.boldCharacter = false;
	// End table cell
	_rtf.end_tablecell();
	// End table row
	_rtf.end_tablerow();
	// Start table row
	_rtf.start_tablerow();
	// Format table cell
	cf->borderLeft.BORDERS.borderWidth = 5;
	cf->SHADING.shadingBkColor = 3;
	// Start table cell
	_rtf.start_tablecell(4000);
	cf->cellShading = false;
	// Write paragraph text
	_rtf.start_paragraph( "This text is in another row, and these cells are merged...", false );
	pf->tableText = false;
	// End table cell
	_rtf.end_tablecell();
	// End table row
	_rtf.end_tablerow();

	// Close RTF file
	_rtf.close();

	return 0;
}

