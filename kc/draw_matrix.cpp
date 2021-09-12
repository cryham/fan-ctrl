#include "gui.h"
#include "Ada4_ST7735.h"
#include "matrix.h"
#include "kc_data.h"


//  Draw Matrix
//....................................................................................
void Gui::DrawMatrix()
{
	char a[64];

	//  title
	d->setClr(12,20,28);
	d->setFont(0);
	d->setClr(21,26,31);


	//-----------------------------------------------------
	const static int16_t x1 = 10, y1 = 64;
	uint c,r, hc=99,hr=99;  // held col,row
	const uint X = 9;

	//  matrix  :::  *
	for (c=0; c < NumCols; ++c)
	for (r=0; r < NumRows; ++r)
	{
		d->setCursor(x1 + c*X, y1 + r*8);
		const KeyState& k = Matrix_scanArray[NumCols * r + c];

		a[1]=0;
		if (k.state == KeyState_Off)
		{	//sprintf(a,".");
			a[0] = 2;
		}else{
			hc = c; hr = r;  // held
			a[0] = '*';
			//sprintf(a,"%d", k.state);
			d->setClr(24,28,31);
		}
		d->print(a);
	}

	//  held  ---
	d->setCursor(0,26);
	d->setClr(24,24,31);
	sprintf(a,"Held %d  press %d ", cnt_press-cnt_rel, cnt_press);
	d->print(a);
	if  (hc<99 && hr<99)
	{
		d->setCursor(W/2, 6);
		sprintf(a,"R%d C%d  %d", hr+1, hc+1, NumCols * hr + hc);
		d->print(a);
	}
}
