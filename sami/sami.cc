/*
 *	subtitleeditor -- a tool to create or edit subtitle
 *
 *	http://home.gna.org/subtitleeditor/
 *	https://gna.org/projects/subtitleeditor/
 *
 *	Copyright @ 2005-2009, kitone
 *	Copyright @ 2011, advance38
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <string.h>
#include <extension/subtitleformat.h>
#include <utility.h>
#include <error.h>
#include <libxml++/libxml++.h>

const static int MAXBUF = 1024;
const static char STARTATT[] = "start=";
const static char BRTAG[] = "<br>";
const static char CRCHAR = '\r';
const static char LFCHAR = '\n';

enum sami_state_t
{
	SAMI_STATE_INIT = 0,
	SAMI_STATE_SYNC_START = 1,
	SAMI_STATE_P_OPEN = 2,
	SAMI_STATE_P_CLOSE = 3,
	SAMI_STATE_SYNC_END = 4,
	SAMI_STATE_FORCE_QUIT = 99
};

/*
 * format of sami (.smi) subtitles:
 *
 * <SAMI>
 * <BODY>
 * <SYNC Start=starttime>text
 * <SYNC Start=endtime>&nbsp;
 * (empty line)
 */
class Sami : public SubtitleFormatIO
{
public:

	/*
	 * open()
	 *  : reads sami subtitle data from the handler 'file', parse each line,
	 *    and store it to the internal data structure 'subtitles'.
	 */
	void open(FileReader &file)
	{
		Subtitles subtitles = document()->subtitles();

		unsigned long startSync = 0, endSync = 0;
		int state = 0;
		Glib::ustring line;
		Glib::ustring text;
		Subtitle* curSt;
		char tmptext[MAXBUF+1] = "";
		char *p = NULL;

		if (!file.getline(line))
			return;

		char *inptr = (char *)(line.c_str());

		do
		{
			switch(state)
			{

			case SAMI_STATE_INIT:
				inptr = strcasestr(inptr, STARTATT);
				if (inptr) {
					startSync = utility::string_to_int(inptr + 6);
					//std::cout << "startSync = " << startSync << std::endl;

					// Get a line from the current subtitle on memory
					curSt = &subtitles.append();
					curSt->set_start(startSync);

					state = SAMI_STATE_SYNC_START;
					continue;
				}
				break;

			case SAMI_STATE_SYNC_START:	// find "<P"
				if ((inptr = strcasestr (inptr, "<P")))
				{
					inptr += 2;
					state = SAMI_STATE_P_OPEN;
					continue;
				}
				break;

			case SAMI_STATE_P_OPEN:	// find ">"
				if ((inptr = strchr (inptr, '>')))
				{
					inptr++;
					state = SAMI_STATE_P_CLOSE;
					p = tmptext;
					continue;
				}
				break;

			case SAMI_STATE_P_CLOSE:	// get all text until '<' appears
				if (*inptr == '\0')
					break;
				else if (strncasecmp (inptr, "&nbsp;", 6) == 0)
				{
					*p++ = ' ';
					inptr += 6;
				}
				else if (strncasecmp (inptr, "nbsp;", 5) == 0)
				{
					*p++ = ' ';
					inptr += 5;
				}
				else if (*inptr == CRCHAR)
					inptr++;
				else if (strncasecmp (inptr, BRTAG, 4) == 0 || *inptr == LFCHAR)
				{
					*p++ = LFCHAR;
					trail_space(inptr);
					if (*inptr == LFCHAR)
						inptr++;
					else
						inptr += 4;
				}
				else if (*inptr == '<')
					state = SAMI_STATE_SYNC_END;
				else
					*p++ = *inptr++;

				continue;

			case SAMI_STATE_SYNC_END:	// get the line for end sync or skip <TAG>
				{
					char *q = strcasestr(inptr, STARTATT);
					if (q)
					{
						endSync = utility::string_to_int(q + 6);
						//std::cout << "endSync = " << endSync << std::endl;
						curSt->set_end(endSync);

						*p = '\0';
						trail_space(tmptext);

						if (tmptext[0] != '\0')
							curSt->set_text(tmptext);

						if (file.getline(line))
						{
							inptr = (char *)(line.c_str());
							p = tmptext;
							p = '\0';

							state = SAMI_STATE_INIT;
							continue;
						}
						else
						{
							state = SAMI_STATE_FORCE_QUIT;
							break;
						}
					}
				}

				inptr = strchr (inptr, '>');
				if (inptr)
				{
					inptr++;
					state = 3;
					continue;
				}
				break;
			}	// end of switch

			// read next line
			if (state != SAMI_STATE_FORCE_QUIT && !file.getline(line))
				return;

			inptr = (char *)(line.c_str());

		} while(state != SAMI_STATE_FORCE_QUIT);
	}


	/*
	 * save()
	 *  : store each line from the internal data structure, in according to the SAMI format.
	 */
	void save(FileWriter &file)
	{
	}

	/*
	 * trail_space()
	 *  : trim the trailing white space characters in s.
	 */
	void trail_space(char *s)
	{
		while (isspace(*s))
		{
			char *copy = s;
			do {
				copy[0] = copy[1];
				copy++;
			} while(*copy);
		}
		size_t i = strlen(s) - 1;
		while (i > 0 && isspace(s[i]))
			s[i--] = '\0';
	}

	/*
	 * time_to_sami
	 *  :
	 */
	Glib::ustring time_to_sami(const SubtitleTime &t)
	{
		unsigned int total_sec = (t.hours() * 3600) + (t.minutes() * 60) + t.seconds();

		return build_message("%i%03i", total_sec, t.mseconds());
						//	"%02i:%02i:%02i,%03i",
						//	t.hours(), t.minutes(), t.seconds(), t.mseconds());
	}

	/*
	 * read_subtitle
	 */

	void read_subtitle()
	{
	}

	/*
	 * write_subtitle
	 */
	void write_subtitle()
	{
	}
};

class SamiPlugin : public SubtitleFormat
{
public:

	/*
	 *
	 */
	SubtitleFormatInfo get_info()
	{
		SubtitleFormatInfo info;
		info.name = "Sami";
		info.extension = "smi";
		info.pattern = "^<SAMI>";
		
		return info;
	}

	/*
	 *
	 */
	SubtitleFormatIO* create()
	{
		Sami *sf = new Sami();
		return sf;
	}
};

REGISTER_EXTENSION(SamiPlugin)
