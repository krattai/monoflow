/* FORM.C: Manipulates screen as form

	Created by:		Kevin Rattai
	Date created:	May 1, 1993

	Modification:	August 12, 1993
	Description:	Made changes to allow user to get information from the
						forms.
					Added Parse_coded_info().
	Modified by:	Kevin Rattai

*/

#include <stdio.h>
#include <conio.h>
#include "form.h"
#include "keyio.h"

/* Display form to user screen */
int Show_form(char form_string[])
{
	char	cx[3] = "",
			cy[3] = "",
			message[79] = "",
			*form,
			char_val;
	int		x,
			y,
			where,
			pos,
			form_condition = 0;

	CLS;
	form = form_string;
	where = 0;
	pos = 0;

	/* parse form information until no more info */
	while(*form != NULL)
	{

		/* end of sub info string */
		if(*form == ',')
		{

			/* which info type was parsed */
			switch(where)
			{
				case 0:	x = atoi(&cx);
						where++;
						break;
				case 1:	y = atoi(&cy);
						where++;
						break;
				case 2: where = 0;
						gotoxy(x,y);
						cprintf("%s",message);
						for(pos = 0; pos <= 3; pos++) cx[pos] = NULL;
						for(pos = 0; pos <= 3; pos++) cy[pos] = NULL;
						for(pos = 0; pos <= 79; pos++) message[pos] = NULL;
			}
			pos = 0;
		}
		else
		{
			/* which type of info to parse */
			switch(where)
			{
				case 0:	cx[pos] = *form;
						break;
				case 1:	cy[pos] = *form;
						break;
				case 2:	message[pos] = *form;
			}
			pos++;
		}
		form++;
	}
	x = atoi(&cx);
	y = atoi(&cy);
	gotoxy(x,y);
	cprintf("%s",message);
	return(form_condition);
}

/* Put information to screen */
int Put_form_info(char form_string[], char form_info[])
{
	char	cx[3] = "",
			cy[3] = "",
			put_mess[79] = "",
			put_info[79] = "",
			*form,
			*info,
			char_val;
	int		x,
			y,
			where,
			pos,
			info_pos = 0,
			length,
			form_condition = 0;

	form = form_string;
	info = form_info;
	where = 0;
	pos = 0;

	/* parse form to find data positions as per Show_form() */
	while(*form != NULL)
	{
		if(*form == ',')
		{

			switch(where)
			{
				case 0:	x = atoi(&cx);
						where++;
						break;
				case 1:	y = atoi(&cy);
						where++;
						break;
				case 2:	for(pos = 0; pos <= 79; pos++) put_info[pos] = NULL;
						pos = 0;
						while(info[info_pos] != SPLIT && info[info_pos] != NULL)
						{
							put_info[pos] = info[info_pos];
							pos++;
							info_pos++;
						}
						info_pos++;
						where = 0;
						length = strlen(put_mess);
						gotoxy(x+length+1,y);
						cprintf("%s",put_info);
						for(pos = 0; pos <= 3; pos++) cx[pos] = NULL;
						for(pos = 0; pos <= 3; pos++) cy[pos] = NULL;
						for(pos = 0; pos <= 79; pos++) put_mess[pos] = NULL;
			}
			pos = 0;
		}
		else
		{
			switch(where)
			{
				case 0:	cx[pos] = *form;
						break;
				case 1:	cy[pos] = *form;
						break;
				case 2:	put_mess[pos] = *form;
			}
			pos++;
		}
		form++;
	}
	x = atoi(&cx);
	y = atoi(&cy);
	for(pos = 0; pos <= 79; pos++) put_info[pos] = NULL;
	pos = 0;
	while(info[info_pos] != SPLIT && info[info_pos] != NULL)
	{
		put_info[pos] = info[info_pos];
		pos++;
		info_pos++;
	}
	info_pos++;
	length = strlen(put_mess);
	gotoxy(x+length+1,y);
	cprintf("%s",put_info);
	return(form_condition);
}

/* Get information from screen */
int Get_form_info(char form_string[], char* info)
{
	char	cx[3] = "",
			cy[3] = "",
			put_mess[79] = "",
			fbuf[79] = "",
			*form,
			*temp,
			char_val;
	int		x,
			y,
			where,
			pos,
			length,
			form_condition = 0,
			key,
			num_fields = 0,
			field_pos[50][2],
			field_len[50];

	form = form_string;
	where = 0;
	pos = 0;

	while(*form != NULL)
	{
		if(*form == ',')
		{

			switch(where)
			{
				case 0:	where++;
						break;
				case 1:	where++;
						break;
				case 2:	where = 0;
						length = strlen(put_mess);
						field_pos[num_fields][0] = atoi(&cx) + length + 1;
						field_pos[num_fields][1] = atoi(&cy);
						num_fields++;
						for(pos = 0; pos <= 3; pos++) cx[pos] = NULL;
						for(pos = 0; pos <= 3; pos++) cy[pos] = NULL;
						for(pos = 0; pos <= 79; pos++) put_mess[pos] = NULL;
			}
			pos = 0;
		}
		else
		{
			switch(where)
			{
				case 0:	cx[pos] = *form;
						break;
				case 1:	cy[pos] = *form;
						break;
				case 2:	put_mess[pos] = *form;
			}
			pos++;
		}
		form++;
	}
	length = strlen(put_mess);
	field_pos[num_fields][0] = atoi(&cx) + length + 1;
	field_pos[num_fields][1] = atoi(&cy);

	/* set up current length of input in fields */
	/* check whether data exists */
	temp = info;
	x = 0;
	while(*info!=NULL)
	{
		if(*info == SPLIT) x = 1;
		info++;
	}
	info = temp;
	if(x)
	{
		for(pos=0;pos<num_fields;pos++)
		{
			length=0;
			while(*info!=SPLIT)
			{
				length++;
				info++;
			}
			info++;
			field_len[pos]=length;
		}
		length=0;
		while(*info!=NULL)
		{
			length++;
			info++;
		}
		field_len[pos]=length;
		length=field_len[0];
		info=temp;
		Put_form_info(form_string,info);
		Un_parse_info(info);
	}
	else
	{
		length=0;
		for(x=0;x<=num_fields;x++) field_len[x]=length;
	}

	/* set up to start at field 0 */
	x = 0;
	gotoxy(field_pos[x][0]+field_len[x],field_pos[x][1]);

	where=0;
	/* may not need this clear */
	for(pos=0;pos<=79;pos++)
		fbuf[pos]=NULL;

	for(pos=0;pos<x;pos++)
		where+=field_len[pos];
	for(pos=0;pos<length;pos++)
		fbuf[pos]=info[where+pos];
	if(length==0)
		fbuf[0]=NULL;

	/* get keystrokes until F2 (END) */
	key = getkey();
	while(key != F2KEY)
	{
		switch(key)
		{
			case ENTER:
			case ASCTAB:	where=0;

							/* find beginning of current field */
							for(pos=0;pos<x;pos++)
								where+=field_len[pos];
							temp = info;
							info+=where;

							/* is new value longer */
							if(length>field_len[x])
							{
								/* go to end of info and make more room */
								pos=0;
								while(*info!=NULL)
								{
									info++;
									pos++;
								}
								info-=pos;
								/* length -= field_len[x]; */
								while(pos>=0)
								{
									/* calc length=length-field_len[x]
									   then calc back; faster			*/
									*(info+pos+length-field_len[x])
														= *(info + pos);
									pos--;
								}
								/* length += field_len[x]; */
							}

							/* is new value shorter */
							if(length<field_len[x])
							{
								/* shrink to size */
								while(*info!=NULL)
								{
									*(info) = *(info+field_len[x]-length);
									info++;
								}
								info = temp+where;
							}

							/* insert new value */
							if(length)
							{
								for(pos=0;pos<length;pos++)
								{
									*info=fbuf[pos];
									info++;
								}
							}

							/* put info back to start */
							info=temp;

							field_len[x] = length;

							/* change fields */
							if(x < num_fields)
							{
								x++;
							}
							else
							{
								x = 0;
							}
							length=field_len[x];

							where=0;
							/* may not need this clear */
							for(pos=0;pos<=79;pos++)
								fbuf[pos]=NULL;

							for(pos=0;pos<x;pos++)
								where+=field_len[pos];
							for(pos=0;pos<length;pos++)
								fbuf[pos]=info[where+pos];
							if(length==0)
								fbuf[0]=NULL;

							gotoxy(field_pos[x][0]+length,field_pos[x][1]);
							break;
			case BACKTAB:	where=0;

							/* find beginning of current field */
							for(pos=0;pos<x;pos++)
								where+=field_len[pos];
							temp = info;
							info+=where;

							/* is new value longer */
							if(length>field_len[x])
							{
								/* go to end of info and make more room */
								pos=0;
								while(*info!=NULL)
								{
									info++;
									pos++;
								}
								info-=pos;
								/* length -= field_len[x]; */
								while(pos>=0)
								{
									/* calc length=length-field_len[x]
									   then calc back; faster			*/
									*(info+pos+length-field_len[x])
														= *(info + pos);
									pos--;
								}
								/* length += field_len[x]; */
							}

							/* is new value shorter */
							if(length<field_len[x])
							{
								/* shrink to size */
								while(*info!=NULL)
								{
									*(info) = *(info+field_len[x]-length);
									info++;
								}
								info = temp+where;
							}

							/* insert new value */
							if(length)
							{
								for(pos=0;pos<length;pos++)
								{
									*info=fbuf[pos];
									info++;
								}
							}

							/* put info back to start */
							info=temp;

							field_len[x] = length;

							/* change fields */
							if(x > 0)
							{
								x--;
							}
							else
							{
								x = num_fields;
							}
							length=field_len[x];

							where=0;
							/* may not need this clear */
							for(pos=0;pos<=79;pos++)
								fbuf[pos]=NULL;

							for(pos=0;pos<x;pos++)
								where+=field_len[pos];
							for(pos=0;pos<length;pos++)
								fbuf[pos]=info[where+pos];
							if(length==0)
								fbuf[0]=NULL;

							gotoxy(field_pos[x][0]+length,field_pos[x][1]);
							break;
			case BACKSPACE:	if(length)
							{
								cprintf("%c",0x8);
								cprintf("%c",' ');
								cprintf("%c",0x8);
								length--;
								fbuf[length] = NULL;
							}
							else
							{
								putchar(7);
							}
							break;
			default: if(key >= ' ' && key <= '~')
							{
								cprintf("%c",key);
								fbuf[length] = key;
								length++;
							}
							else
							{
								putchar(7);
							}
		}
		key = getkey();
	}

	where=0;

	/* find beginning of current field */
	for(pos=0;pos<x;pos++)
		where+=field_len[pos];
	temp = info;
	info+=where;

	/* is new value longer */
	if(length>field_len[x])
	{
		/* go to end of info and make more room */
		pos=0;
		while(*info!=NULL)
		{
			info++;
			pos++;
		}
		info-=pos;
		/* length -= field_len[x]; */
		while(pos>=0)
		{
			/* calc length=length-field_len[x]
			   then calc back; faster			*/
			*(info+pos+length-field_len[x])
								= *(info + pos);
			pos--;
		}
		/* length += field_len[x]; */
	}

	/* is new value shorter */
	if(length<field_len[x])
	{
		/* shrink to size */
		while(*info!=NULL)
		{
			*(info) = *(info+field_len[x]-length);
			info++;
		}
		info = temp+where;
	}

	/* insert new value */
	if(length)
	{
		for(pos=0;pos<length;pos++)
		{
			*info=fbuf[pos];
			info++;
		}
	}

	/* put info back to start */
	info=temp;

	field_len[x]=length;

	/* insert spacing information */
	where=0;
	for(pos=0;pos<num_fields;pos++)
	{
		where+=field_len[pos];
		x=0;
		info = temp+where;
		while(*info!=NULL)
		{
			x++;
			info++;
		}
		info-=x;
		while(x)
		{
			*(info+x+1)=*(info+x);
			x--;
		}
		*(info+1)=*(info);
		*info = SPLIT;
		where++;
	}

	return(form_condition);
}

/* Wait on screen while user enters information */
int Wait_form(char form_string, int conditions[])
/* int Wait_form(char form_string, int conditions[], int values[]) */
{
	char	cx[3] = "",
			cy[3] = "",
			put_mess[79] = "",
			put_info[79] = "",
			*form,
			*info,
			char_val,
			key;
	int		x,
			y,
			where,
			pos,
			info_pos = 0,
			length,
			form_condition = 0;

/*	form = form_string;
	info = form_info;
	where = 0;
	pos = 0;

	key = getch();
	while(key != F2)
	{
		cprintf("Not F2\n");
	} */

/*	while(*form != NULL)
	{
		if(*form == ',')
		{

			switch(where)
			{
				case 0:	x = atoi(&cx);
						where++;
						break;
				case 1:	y = atoi(&cy);
						where++;
						break;
				case 2:	for(pos = 0; pos <= 79; pos++) put_info[pos] = NULL;
						pos = 0;
						while(info[info_pos] != ',' && info[info_pos] != NULL)
						{
							put_info[pos] = info[info_pos];
							pos++;
							info_pos++;
						}
						info_pos++;
						where = 0;
						length = strlen(put_mess);
						gotoxy(x+length+1,y);
						cprintf("%s",put_info);
						for(pos = 0; pos <= 3; pos++) cx[pos] = NULL;
						for(pos = 0; pos <= 3; pos++) cy[pos] = NULL;
						for(pos = 0; pos <= 79; pos++) put_mess[pos] = NULL;
			}
			pos = 0;
		}
		else
		{
			switch(where)
			{
				case 0:	cx[pos] = *form;
						break;
				case 1:	cy[pos] = *form;
						break;
				case 2:	put_mess[pos] = *form;
			}
			pos++;
		}
		form++;
	}
	x = atoi(&cx);
	y = atoi(&cy);
	for(pos = 0; pos <= 79; pos++) put_info[pos] = NULL;
	pos = 0;
	while(info[info_pos] != ',' && info[info_pos] != NULL)
	{
		put_info[pos] = info[info_pos];
		pos++;
		info_pos++;
	}
	info_pos++;
	length = strlen(put_mess);
	gotoxy(x+length+1,y);
	cprintf("%s",put_info); */

/* condition[0] = NOT
condition[1] = EQUAL */
/*   key = getch();
	 key = F3;

	 if(key condition[0] condition[1] vaule[0] &&
		key condition[0] condition[1] value[1])

	 if(F3 ! = F1 &&
		F3 ! = F2)
	 {
*/
	return(1);
}

int Get_form_string(char *string, int length)
{
	int	counter,
		fini;

	counter = 0;
	fini = 0;
	while(!fini)
	{
		string[counter] = toupper(getch());
		switch(string[counter])
		{
			case NULL:	string[counter] = toupper(getch());
						switch((int) string[counter] *-1)
						{
							case LEFT:	counter--;
										gotoxy(wherex()-1,wherey());
										putch(' ');
										gotoxy(wherex()-1,wherey());
										break;
							default:	putchar(7);
										break;
						}
						break;
			case ENTER:	string[counter] = NULL;
						fini=1;
						break;
			case ASCBS:	counter--;
						gotoxy(wherex()-1,wherey());
						putch(' ');
						gotoxy(wherex()-1,wherey());
						break;
			default:    string[counter] = toupper(string[counter]);
						putch(string[counter]);
						counter++;
						if(counter >= length) fini = 1;
						break;
		}
	}
	return(counter);
}

int Put_window(int left, int top, int right, int bot, int text, int back)
{
	textcolor(text);
	textbackground(back);
	window(left, top, right, bot);
	return(1);
}

int Put_original(struct text_info original)
{
	textattr(original.attribute);
	window(original.winleft,
			original.wintop,
			original.winright,
			original.winbottom);
	return(1);
}

int Parse_coded_info(char *string)
{
	int	counter=0;
	char split;

	split = string[counter];
	counter++;
	while(string[counter] != NULL)
	{
		if(string[counter] == split)
		{
			string[counter-1] = SPLIT;
		}
		else
		{
			string[counter-1] = string[counter];
		}
		counter++;
	}
	string[counter-1] = string[counter];

	return(1);
}

int Un_parse_info(char *string)
{
	int	true=1;
	char *temp;

	temp = string;

	while(true)
	{
		while(*temp!=SPLIT && true)
		{
			if(*temp==NULL)
				true=0;
			temp++;
		}
		if(true)
		{
			while(*temp!=NULL)
			{
				*temp=*(temp+1);
				temp++;
			}
		}
		temp = string;
	}

	return(1);
}

int Load_dict(void)
{
	/* This function would be called with either a string of forms info
			or a file with all the forms info
		It would load the dictionary with all relevant information
			pertaining to each form
		The information would then be available for retreval from the system
			whenever required by making a call to the dictionary with a
			specific form number
		It could also be somehow linked or related to a file structure
			dictionary which could then retrieve important file info */
}