<?php
if(extension_loaded("iconv")){
echo "Iconv loaded:";
echo  "xxxxxx=".iconv_strlen("xxxxxx")."<br/>";

}
else{
echo "Iconv  notloaded<br/>";
die;
}

if(extension_loaded("excel")){
//echo phpinfo();
echo "Excel loaded\n";
$excel=Excel::create(1, 'UTF-8');
$font	= new ExcelFont(ExcelFont::WEIGHT_BOLD);
	$font->setColor(ExcelFont::COLOR_RED);
$font->setFontName("Times New Roman");
//$font->setFontName("Athelas Regular");
$font->setFontSize(20);
$font->setUnderLine(true);

	$cellFmt	= new ExcelCellFormat($excel);
	$cellFmt2	= new ExcelCellFormat($excel);
	$cellFmt3	= new ExcelCellFormat($excel);
	$cellFmt->setFont($font);
/*
EXCEL_HALIGN_GENERAL		= 0x00,
	EXCEL_HALIGN_LEFT			= 0x01,
	EXCEL_HALIGN_CENTRED		= 0x02,
	EXCEL_HALIGN_RIGHT			= 0x03,
	EXCEL_HALIGN_FILLED			= 0x04,
	EXCEL_HALIGN_JUSITFIED		= 0x05,
	EXCEL_HALIGN_SEL_CENTRED	= 0x06,	// centred across selection
	EXCEL_HALIGN_DISTRIBUTED	= 0x07,	// a
	
	*/
	$cellFmt->setAlignment(0x03);
//		$cellFmt->setBackGround(1,0x04000000);
		
		/*
		
		color2:
		COLOR1_PAT_EMPTY		= 0x00000000,
	COLOR1_PAT_SOLID		= 0x04000000,
	COLOR1_PAT_2			= 0x08000000,
	COLOR1_PAT_3			= 0x0C000000,
	COLOR1_PAT_4			= 0x10000000,
	COLOR1_PAT_5			= 0x14000000,
	COLOR1_PAT_6			= 0x18000000,
	COLOR1_PAT_7			= 0x1C000000,
	COLOR1_PAT_8			= 0x20000000,
	COLOR1_PAT_9			= 0x24000000,
	COLOR1_PAT_10			= 0x28000000,
	COLOR1_PAT_11			= 0x2C000000,
	COLOR1_PAT_12			= 0x30000000,
	COLOR1_PAT_13			= 0x34000000,
	COLOR1_PAT_14			= 0x38000000,
	COLOR1_PAT_15			= 0x3C000000,
	COLOR1_PAT_16			= 0x40000000,
	COLOR1_PAT_17			= 0x44000000,
	COLOR1_PAT_18			= 0x48000000*/
		$sheet	= $excel->getWorkSheet(0);
for ($row = 0; $row < 60;  $row++)
	{
		for ($col = 0; $col < 30;  $col++)
		{ 
//			$sheet->setWString( $col, $row, rand(),$cellFmt);
			$sheet->setAnsiString($col, $row,rand(),$cellFmt); 
		}
	}
	echo "start";
	$cellFmt2->setAlignment(0x01);
//		$cellFmt2->setBackGround(100100,200200);

echo "ASDDS";
	$cellFmt3->setAlignment(0x02);
//	$cellFmt3->setBackGround(100100,200200);

	$sheet->setFormat( 0, 0,$cellFmt2);
	$sheet->setWString( 0, 0, "Set col0,row0 value");
	$sheet->setFormat( 5, 5,$cellFmt3);
	$sheet->setWString( 5, 5, "S222et col6,row6 value");
//	$sheet->mergeCells(2,4,6,8);
//	$sheet->setFormat( 4, 1,$cellFmt3);
	$sheet->setColWidth(1,10);
	$excel->saveAs('/tmp/bbb.xls');
	echo gettimeofday(true)  . ' sec';
}
else
echo "no excel\n";

