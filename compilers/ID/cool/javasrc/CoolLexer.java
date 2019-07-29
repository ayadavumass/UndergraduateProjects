/*
 *  The scanner definition for COOL.
 *
 *  Scott McPeak (sm) 1/11/01:
 *    - changed to student-implemented line counting
 *    - fixed another \v bug (jlex doesn't interpret \v)
 *
 * sm 1/27/01:
 *    - error characters are now *not* coalesced (see src/cool.flex)
 */
import java_cup.runtime.Symbol;


class CoolLexer implements java_cup.runtime.Scanner {
	private final int YY_BUFFER_SIZE = 512;
	private final int YY_F = -1;
	private final int YY_NO_STATE = -1;
	private final int YY_NOT_ACCEPT = 0;
	private final int YY_START = 1;
	private final int YY_END = 2;
	private final int YY_NO_ANCHOR = 4;
	private final int YY_BOL = 128;
	private final int YY_EOF = 129;

    // Max size of string constants
    static int MAX_STR_CONST = 1025;
    int comment_nesting = 0;
    // For assembling string constants
    StringBuffer string_buf = new StringBuffer();
    Symbol check_string_overflow() {
        // sm: fixed another off-by-one: this used to say '>'
	if (string_buf.length() >= MAX_STR_CONST) {
	    yybegin(STRING_RECOVER);
	    return new Symbol(TokenConstants.ERROR, "String constant too long");
	}
	return null;
    }
    private int curr_lineno = 1;
    int get_curr_lineno() {
	return curr_lineno;
    }
    private AbstractSymbol filename;
    void set_filename(String fname) {
	filename = AbstractTable.stringtable.addString(fname);
    }
    AbstractSymbol curr_filename() {
	return filename;
    }
	private java.io.BufferedReader yy_reader;
	private int yy_buffer_index;
	private int yy_buffer_read;
	private int yy_buffer_start;
	private int yy_buffer_end;
	private char yy_buffer[];
	private boolean yy_at_bol;
	private int yy_lexical_state;

	CoolLexer (java.io.Reader reader) {
		this ();
		if (null == reader) {
			throw (new Error("Error: Bad input stream initializer."));
		}
		yy_reader = new java.io.BufferedReader(reader);
	}

	CoolLexer (java.io.InputStream instream) {
		this ();
		if (null == instream) {
			throw (new Error("Error: Bad input stream initializer."));
		}
		yy_reader = new java.io.BufferedReader(new java.io.InputStreamReader(instream));
	}

	private CoolLexer () {
		yy_buffer = new char[YY_BUFFER_SIZE];
		yy_buffer_read = 0;
		yy_buffer_index = 0;
		yy_buffer_start = 0;
		yy_buffer_end = 0;
		yy_at_bol = true;
		yy_lexical_state = YYINITIAL;

    // empty now
	}

	private boolean yy_eof_done = false;
	private final int STRING = 3;
	private final int STRING_RECOVER = 4;
	private final int LINE_COMMENT = 2;
	private final int YYINITIAL = 0;
	private final int COMMENT = 1;
	private final int yy_state_dtrans[] = {
		0,
		77,
		56,
		98,
		101
	};
	private void yybegin (int state) {
		yy_lexical_state = state;
	}
	private int yy_advance ()
		throws java.io.IOException {
		int next_read;
		int i;
		int j;

		if (yy_buffer_index < yy_buffer_read) {
			return yy_buffer[yy_buffer_index++];
		}

		if (0 != yy_buffer_start) {
			i = yy_buffer_start;
			j = 0;
			while (i < yy_buffer_read) {
				yy_buffer[j] = yy_buffer[i];
				++i;
				++j;
			}
			yy_buffer_end = yy_buffer_end - yy_buffer_start;
			yy_buffer_start = 0;
			yy_buffer_read = j;
			yy_buffer_index = j;
			next_read = yy_reader.read(yy_buffer,
					yy_buffer_read,
					yy_buffer.length - yy_buffer_read);
			if (-1 == next_read) {
				return YY_EOF;
			}
			yy_buffer_read = yy_buffer_read + next_read;
		}

		while (yy_buffer_index >= yy_buffer_read) {
			if (yy_buffer_index >= yy_buffer.length) {
				yy_buffer = yy_double(yy_buffer);
			}
			next_read = yy_reader.read(yy_buffer,
					yy_buffer_read,
					yy_buffer.length - yy_buffer_read);
			if (-1 == next_read) {
				return YY_EOF;
			}
			yy_buffer_read = yy_buffer_read + next_read;
		}
		return yy_buffer[yy_buffer_index++];
	}
	private void yy_move_end () {
		if (yy_buffer_end > yy_buffer_start &&
		    '\n' == yy_buffer[yy_buffer_end-1])
			yy_buffer_end--;
		if (yy_buffer_end > yy_buffer_start &&
		    '\r' == yy_buffer[yy_buffer_end-1])
			yy_buffer_end--;
	}
	private boolean yy_last_was_cr=false;
	private void yy_mark_start () {
		yy_buffer_start = yy_buffer_index;
	}
	private void yy_mark_end () {
		yy_buffer_end = yy_buffer_index;
	}
	private void yy_to_mark () {
		yy_buffer_index = yy_buffer_end;
		yy_at_bol = (yy_buffer_end > yy_buffer_start) &&
		            ('\r' == yy_buffer[yy_buffer_end-1] ||
		             '\n' == yy_buffer[yy_buffer_end-1] ||
		             2028/*LS*/ == yy_buffer[yy_buffer_end-1] ||
		             2029/*PS*/ == yy_buffer[yy_buffer_end-1]);
	}
	private java.lang.String yytext () {
		return (new java.lang.String(yy_buffer,
			yy_buffer_start,
			yy_buffer_end - yy_buffer_start));
	}
	private int yylength () {
		return yy_buffer_end - yy_buffer_start;
	}
	private char[] yy_double (char buf[]) {
		int i;
		char newbuf[];
		newbuf = new char[2*buf.length];
		for (i = 0; i < buf.length; ++i) {
			newbuf[i] = buf[i];
		}
		return newbuf;
	}
	private final int YY_E_INTERNAL = 0;
	private final int YY_E_MATCH = 1;
	private java.lang.String yy_error_string[] = {
		"Error: Internal error.\n",
		"Error: Unmatched input.\n"
	};
	private void yy_error (int code,boolean fatal) {
		java.lang.System.out.print(yy_error_string[code]);
		java.lang.System.out.flush();
		if (fatal) {
			throw new Error("Fatal Error.\n");
		}
	}
	private int[][] unpackFromString(int size1, int size2, String st) {
		int colonIndex = -1;
		String lengthString;
		int sequenceLength = 0;
		int sequenceInteger = 0;

		int commaIndex;
		String workString;

		int res[][] = new int[size1][size2];
		for (int i= 0; i < size1; i++) {
			for (int j= 0; j < size2; j++) {
				if (sequenceLength != 0) {
					res[i][j] = sequenceInteger;
					sequenceLength--;
					continue;
				}
				commaIndex = st.indexOf(',');
				workString = (commaIndex==-1) ? st :
					st.substring(0, commaIndex);
				st = st.substring(commaIndex+1);
				colonIndex = workString.indexOf(':');
				if (colonIndex == -1) {
					res[i][j]=Integer.parseInt(workString);
					continue;
				}
				lengthString =
					workString.substring(colonIndex+1);
				sequenceLength=Integer.parseInt(lengthString);
				workString=workString.substring(0,colonIndex);
				sequenceInteger=Integer.parseInt(workString);
				res[i][j] = sequenceInteger;
				sequenceLength--;
			}
		}
		return res;
	}
	private int yy_acpt[] = {
		/* 0 */ YY_NOT_ACCEPT,
		/* 1 */ YY_NO_ANCHOR,
		/* 2 */ YY_NO_ANCHOR,
		/* 3 */ YY_NO_ANCHOR,
		/* 4 */ YY_NO_ANCHOR,
		/* 5 */ YY_NO_ANCHOR,
		/* 6 */ YY_NO_ANCHOR,
		/* 7 */ YY_NO_ANCHOR,
		/* 8 */ YY_NO_ANCHOR,
		/* 9 */ YY_NO_ANCHOR,
		/* 10 */ YY_NO_ANCHOR,
		/* 11 */ YY_NO_ANCHOR,
		/* 12 */ YY_NO_ANCHOR,
		/* 13 */ YY_NO_ANCHOR,
		/* 14 */ YY_NO_ANCHOR,
		/* 15 */ YY_NO_ANCHOR,
		/* 16 */ YY_NO_ANCHOR,
		/* 17 */ YY_NO_ANCHOR,
		/* 18 */ YY_NO_ANCHOR,
		/* 19 */ YY_NO_ANCHOR,
		/* 20 */ YY_NO_ANCHOR,
		/* 21 */ YY_NO_ANCHOR,
		/* 22 */ YY_NO_ANCHOR,
		/* 23 */ YY_NO_ANCHOR,
		/* 24 */ YY_NO_ANCHOR,
		/* 25 */ YY_NO_ANCHOR,
		/* 26 */ YY_NO_ANCHOR,
		/* 27 */ YY_NO_ANCHOR,
		/* 28 */ YY_NO_ANCHOR,
		/* 29 */ YY_NO_ANCHOR,
		/* 30 */ YY_NO_ANCHOR,
		/* 31 */ YY_NO_ANCHOR,
		/* 32 */ YY_NO_ANCHOR,
		/* 33 */ YY_NO_ANCHOR,
		/* 34 */ YY_NO_ANCHOR,
		/* 35 */ YY_NO_ANCHOR,
		/* 36 */ YY_NO_ANCHOR,
		/* 37 */ YY_NO_ANCHOR,
		/* 38 */ YY_NO_ANCHOR,
		/* 39 */ YY_NO_ANCHOR,
		/* 40 */ YY_NO_ANCHOR,
		/* 41 */ YY_NO_ANCHOR,
		/* 42 */ YY_NO_ANCHOR,
		/* 43 */ YY_NO_ANCHOR,
		/* 44 */ YY_NO_ANCHOR,
		/* 45 */ YY_NO_ANCHOR,
		/* 46 */ YY_NO_ANCHOR,
		/* 47 */ YY_NO_ANCHOR,
		/* 48 */ YY_NO_ANCHOR,
		/* 49 */ YY_NO_ANCHOR,
		/* 50 */ YY_NO_ANCHOR,
		/* 51 */ YY_NO_ANCHOR,
		/* 52 */ YY_NO_ANCHOR,
		/* 53 */ YY_NO_ANCHOR,
		/* 54 */ YY_NO_ANCHOR,
		/* 55 */ YY_NO_ANCHOR,
		/* 56 */ YY_NO_ANCHOR,
		/* 57 */ YY_NO_ANCHOR,
		/* 58 */ YY_NO_ANCHOR,
		/* 59 */ YY_NO_ANCHOR,
		/* 60 */ YY_NO_ANCHOR,
		/* 61 */ YY_NO_ANCHOR,
		/* 62 */ YY_NO_ANCHOR,
		/* 63 */ YY_NO_ANCHOR,
		/* 64 */ YY_NO_ANCHOR,
		/* 65 */ YY_NO_ANCHOR,
		/* 66 */ YY_NO_ANCHOR,
		/* 67 */ YY_NO_ANCHOR,
		/* 68 */ YY_NO_ANCHOR,
		/* 69 */ YY_NO_ANCHOR,
		/* 70 */ YY_NO_ANCHOR,
		/* 71 */ YY_NO_ANCHOR,
		/* 72 */ YY_NO_ANCHOR,
		/* 73 */ YY_NO_ANCHOR,
		/* 74 */ YY_NO_ANCHOR,
		/* 75 */ YY_NO_ANCHOR,
		/* 76 */ YY_NO_ANCHOR,
		/* 77 */ YY_NOT_ACCEPT,
		/* 78 */ YY_NO_ANCHOR,
		/* 79 */ YY_NO_ANCHOR,
		/* 80 */ YY_NO_ANCHOR,
		/* 81 */ YY_NO_ANCHOR,
		/* 82 */ YY_NO_ANCHOR,
		/* 83 */ YY_NO_ANCHOR,
		/* 84 */ YY_NO_ANCHOR,
		/* 85 */ YY_NO_ANCHOR,
		/* 86 */ YY_NO_ANCHOR,
		/* 87 */ YY_NO_ANCHOR,
		/* 88 */ YY_NO_ANCHOR,
		/* 89 */ YY_NO_ANCHOR,
		/* 90 */ YY_NO_ANCHOR,
		/* 91 */ YY_NO_ANCHOR,
		/* 92 */ YY_NO_ANCHOR,
		/* 93 */ YY_NO_ANCHOR,
		/* 94 */ YY_NO_ANCHOR,
		/* 95 */ YY_NO_ANCHOR,
		/* 96 */ YY_NO_ANCHOR,
		/* 97 */ YY_NO_ANCHOR,
		/* 98 */ YY_NOT_ACCEPT,
		/* 99 */ YY_NO_ANCHOR,
		/* 100 */ YY_NO_ANCHOR,
		/* 101 */ YY_NOT_ACCEPT,
		/* 102 */ YY_NO_ANCHOR,
		/* 103 */ YY_NO_ANCHOR,
		/* 104 */ YY_NO_ANCHOR,
		/* 105 */ YY_NO_ANCHOR,
		/* 106 */ YY_NO_ANCHOR,
		/* 107 */ YY_NO_ANCHOR,
		/* 108 */ YY_NO_ANCHOR,
		/* 109 */ YY_NO_ANCHOR,
		/* 110 */ YY_NO_ANCHOR,
		/* 111 */ YY_NO_ANCHOR,
		/* 112 */ YY_NO_ANCHOR,
		/* 113 */ YY_NO_ANCHOR,
		/* 114 */ YY_NO_ANCHOR,
		/* 115 */ YY_NO_ANCHOR,
		/* 116 */ YY_NO_ANCHOR,
		/* 117 */ YY_NO_ANCHOR,
		/* 118 */ YY_NO_ANCHOR,
		/* 119 */ YY_NO_ANCHOR,
		/* 120 */ YY_NO_ANCHOR,
		/* 121 */ YY_NO_ANCHOR,
		/* 122 */ YY_NO_ANCHOR,
		/* 123 */ YY_NO_ANCHOR,
		/* 124 */ YY_NO_ANCHOR,
		/* 125 */ YY_NO_ANCHOR,
		/* 126 */ YY_NO_ANCHOR,
		/* 127 */ YY_NO_ANCHOR,
		/* 128 */ YY_NO_ANCHOR,
		/* 129 */ YY_NO_ANCHOR,
		/* 130 */ YY_NO_ANCHOR,
		/* 131 */ YY_NO_ANCHOR,
		/* 132 */ YY_NO_ANCHOR,
		/* 133 */ YY_NO_ANCHOR,
		/* 134 */ YY_NO_ANCHOR,
		/* 135 */ YY_NO_ANCHOR,
		/* 136 */ YY_NO_ANCHOR,
		/* 137 */ YY_NO_ANCHOR,
		/* 138 */ YY_NO_ANCHOR,
		/* 139 */ YY_NO_ANCHOR,
		/* 140 */ YY_NO_ANCHOR,
		/* 141 */ YY_NO_ANCHOR,
		/* 142 */ YY_NO_ANCHOR,
		/* 143 */ YY_NO_ANCHOR,
		/* 144 */ YY_NO_ANCHOR,
		/* 145 */ YY_NO_ANCHOR,
		/* 146 */ YY_NO_ANCHOR,
		/* 147 */ YY_NO_ANCHOR,
		/* 148 */ YY_NO_ANCHOR,
		/* 149 */ YY_NO_ANCHOR,
		/* 150 */ YY_NO_ANCHOR,
		/* 151 */ YY_NO_ANCHOR,
		/* 152 */ YY_NO_ANCHOR,
		/* 153 */ YY_NO_ANCHOR,
		/* 154 */ YY_NO_ANCHOR,
		/* 155 */ YY_NO_ANCHOR,
		/* 156 */ YY_NO_ANCHOR,
		/* 157 */ YY_NO_ANCHOR,
		/* 158 */ YY_NO_ANCHOR,
		/* 159 */ YY_NO_ANCHOR,
		/* 160 */ YY_NO_ANCHOR,
		/* 161 */ YY_NO_ANCHOR,
		/* 162 */ YY_NO_ANCHOR,
		/* 163 */ YY_NO_ANCHOR,
		/* 164 */ YY_NO_ANCHOR,
		/* 165 */ YY_NO_ANCHOR,
		/* 166 */ YY_NO_ANCHOR,
		/* 167 */ YY_NO_ANCHOR,
		/* 168 */ YY_NO_ANCHOR,
		/* 169 */ YY_NO_ANCHOR,
		/* 170 */ YY_NO_ANCHOR,
		/* 171 */ YY_NO_ANCHOR,
		/* 172 */ YY_NO_ANCHOR,
		/* 173 */ YY_NO_ANCHOR,
		/* 174 */ YY_NO_ANCHOR,
		/* 175 */ YY_NO_ANCHOR,
		/* 176 */ YY_NO_ANCHOR,
		/* 177 */ YY_NO_ANCHOR,
		/* 178 */ YY_NO_ANCHOR,
		/* 179 */ YY_NO_ANCHOR,
		/* 180 */ YY_NO_ANCHOR,
		/* 181 */ YY_NO_ANCHOR,
		/* 182 */ YY_NO_ANCHOR
	};
	private int yy_cmap[] = unpackFromString(1,130,
"63,4:8,5,1,5:2,2,4:18,5,4,60,4:5,6,8,7,50,54,3,52,51,12:10,56,55,11,9,10,4," +
"57,32,33,34,35,36,19,33,37,38,33:2,39,33,40,41,42,33,43,44,24,45,46,47,33:3" +
",4,61,4:2,48,4,14,62,13,27,16,18,49,22,20,49:2,17,49,21,26,28,49,23,15,30,3" +
"1,25,29,49:3,59,4,58,53,4,0:2")[0];

	private int yy_rmap[] = unpackFromString(1,183,
"0,1:2,2,3,1,4,5,1,6,7,8,9,10,1:17,11:2,12,11:8,13,11:7,1,14,15,16,1:2,17,1:" +
"5,18,1:9,19,1,20,1:2,21,22,23,13:2,24,13:8,11,13:5,25,26,27,28,29,30,31,32," +
"33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57," +
"58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82," +
"83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,11,13,101,102,103,10" +
"4,105,106,107,108")[0];

	private int yy_nxt[][] = unpackFromString(109,64,
"1,2,3,4,5,3,6,7,8,9,5,10,11,12,173:2,175,132,78,13,99,134,173:2,79,173,102," +
"173,177,179,181,173,174:2,176,174,178,174,100,133,135,103,180,174:4,182,5,1" +
"73,14,15,16,17,18,19,20,21,22,23,24,5,173,5,-1:66,3,-1:2,3,-1:61,25,-1:67,2" +
"6,-1:64,27,-1:65,28,-1:56,29,-1:5,30,-1:66,11,-1:63,173:2,136,173:2,138,173" +
":14,136,173:6,138,173:10,-1:12,173,-1:13,174:8,80,174:17,80,174:11,-1:12,17" +
"4,-1:13,173:38,-1:12,173,-1:13,173:10,164,173:14,164,173:12,-1:12,173,-1:13" +
",174:38,-1:12,174,-1:3,51:4,-1:2,51:56,-1:7,54,-1:64,55,-1:55,1,57,58,97:61" +
",-1,64,-1,65:15,66,65:2,67,65:8,68,65:31,69,70,-1:2,72:58,-1:2,72:2,-1,75,-" +
"1,76:61,1,50,51:4,52,53,51:56,-1:12,173:2,146,173:5,31,173:11,146,173:5,31," +
"173:11,-1:12,173,-1:13,174:10,137,174:14,137,174:12,-1:12,174,-1:13,174:10," +
"159,174:14,159,174:12,-1:12,174,-1:4,97:61,1,59,-1,60:57,61,62,60,63,-1:12," +
"173:3,148,173:2,32:2,173,33,173:18,33,173:3,148,173:5,-1:12,173,-1:13,174:3" +
",147,174:2,81:2,174,82,174:18,82,174:3,147,174:5,-1:12,174,-1,1,71,72:58,73" +
",74,72:2,-1:12,173:6,34:2,173:30,-1:12,173,-1:13,174:6,83:2,174:30,-1:12,17" +
"4,-1:13,173:12,35,173:5,35,173:19,-1:12,173,-1:13,174:12,84,174:5,84,174:19" +
",-1:12,174,-1:13,173:17,36,173:17,36,173:2,-1:12,173,-1:13,174:17,85,174:17" +
",85,174:2,-1:12,174,-1:13,173:12,37,173:5,37,173:19,-1:12,173,-1:13,174:12," +
"86,174:5,86,174:19,-1:12,174,-1:13,173:4,38,173:19,38,173:13,-1:12,173,-1:1" +
"3,174:9,42,174:18,42,174:9,-1:12,174,-1:13,173,39,173:20,39,173:15,-1:12,17" +
"3,-1:13,174:4,87,174:19,87,174:13,-1:12,174,-1:13,173:4,40,173:19,40,173:13" +
",-1:12,173,-1:13,174,88,174:20,88,174:15,-1:12,174,-1:13,173:16,41,173:13,4" +
"1,173:7,-1:12,173,-1:13,174:4,89,174:19,89,174:13,-1:12,174,-1:13,173:5,43," +
"173:21,43,173:10,-1:12,173,-1:13,174:16,90,174:13,90,174:7,-1:12,174,-1:13," +
"173:9,91,173:18,91,173:9,-1:12,173,-1:13,174:5,92,174:21,92,174:10,-1:12,17" +
"4,-1:13,173:4,44,173:19,44,173:13,-1:12,173,-1:13,174:3,93,174:28,93,174:5," +
"-1:12,174,-1:13,173:3,45,173:28,45,173:5,-1:12,173,-1:13,174:4,94,174:19,94" +
",174:13,-1:12,174,-1:13,173:4,46,173:19,46,173:13,-1:12,173,-1:13,174:15,95" +
",174:7,95,174:14,-1:12,174,-1:13,173:4,47,173:19,47,173:13,-1:12,173,-1:13," +
"174:3,96,174:28,96,174:5,-1:12,174,-1:13,173:15,48,173:7,48,173:14,-1:12,17" +
"3,-1:13,173:3,49,173:28,49,173:5,-1:12,173,-1:13,173:4,104,173:9,144,173:9," +
"104,173:4,144,173:8,-1:12,173,-1:13,174:4,105,174:9,149,174:9,105,174:4,149" +
",174:8,-1:12,174,-1:13,173:4,106,173:9,108,173:9,106,173:4,108,173:8,-1:12," +
"173,-1:13,174:4,107,174:9,109,174:9,107,174:4,109,174:8,-1:12,174,-1:13,173" +
":3,110,173:28,110,173:5,-1:12,173,-1:13,174:4,111,174:19,111,174:13,-1:12,1" +
"74,-1:13,173:2,158,173:17,158,173:17,-1:12,173,-1:13,174:3,113,174:28,113,1" +
"74:5,-1:12,174,-1:13,173:2,112,173:17,112,173:17,-1:12,173,-1:13,174:2,155," +
"174:17,155,174:17,-1:12,174,-1:13,173:3,114,173:28,114,173:5,-1:12,173,-1:1" +
"3,174:2,115,174:17,115,174:17,-1:12,174,-1:13,173:14,116,173:14,116,173:8,-" +
"1:12,173,-1:13,174:3,117,174:28,117,174:5,-1:12,174,-1:13,173:5,160,173:21," +
"160,173:10,-1:12,173,-1:13,174:13,157,174:20,157,174:3,-1:12,174,-1:13,173:" +
"13,162,173:20,162,173:3,-1:12,173,-1:13,174:14,119,174:14,119,174:8,-1:12,1" +
"74,-1:13,173:14,118,173:14,118,173:8,-1:12,173,-1:13,174:14,121,174:14,121," +
"174:8,-1:12,174,-1:13,173:8,166,173:17,166,173:11,-1:12,173,-1:13,174:8,161" +
",174:17,161,174:11,-1:12,174,-1:13,173:4,120,173:19,120,173:13,-1:12,173,-1" +
":13,174:3,123,174:28,123,174:5,-1:12,174,-1:13,173:19,122,173:13,122,173:4," +
"-1:12,173,-1:13,174:14,163,174:14,163,174:8,-1:12,174,-1:13,173:3,124,173:2" +
"8,124,173:5,-1:12,173,-1:13,174:4,165,174:19,165,174:13,-1:12,174,-1:13,173" +
":3,126,173:28,126,173:5,-1:12,173,-1:13,174:5,125,174:21,125,174:10,-1:12,1" +
"74,-1:13,173:14,168,173:14,168,173:8,-1:12,173,-1:13,174:8,127,174:17,127,1" +
"74:11,-1:12,174,-1:13,173:4,170,173:19,170,173:13,-1:12,173,-1:13,174:11,16" +
"7,174:19,167,174:6,-1:12,174,-1:13,173:5,128,173:21,128,173:10,-1:12,173,-1" +
":13,174:8,169,174:17,169,174:11,-1:12,174,-1:13,173:8,130,173:17,130,173:11" +
",-1:12,173,-1:13,174:12,129,174:5,129,174:19,-1:12,174,-1:13,173:11,171,173" +
":19,171,173:6,-1:12,173,-1:13,173:8,172,173:17,172,173:11,-1:12,173,-1:13,1" +
"73:12,131,173:5,131,173:19,-1:12,173,-1:13,173:3,140,173,142,173:21,142,173" +
":4,140,173:5,-1:12,173,-1:13,174:2,139,174:2,141,174:14,139,174:6,141,174:1" +
"0,-1:12,174,-1:13,173:14,150,173:14,150,173:8,-1:12,173,-1:13,174:3,143,174" +
",145,174:21,145,174:4,143,174:5,-1:12,174,-1:13,173:10,152,173:14,152,173:1" +
"2,-1:12,173,-1:13,174:14,151,174:14,151,174:8,-1:12,174,-1:13,173:10,154,15" +
"6,173:13,154,173:5,156,173:6,-1:12,173,-1:13,174:10,153,174:14,153,174:12,-" +
"1:12,174,-1");

	public java_cup.runtime.Symbol next_token ()
		throws java.io.IOException {
		int yy_lookahead;
		int yy_anchor = YY_NO_ANCHOR;
		int yy_state = yy_state_dtrans[yy_lexical_state];
		int yy_next_state = YY_NO_STATE;
		int yy_last_accept_state = YY_NO_STATE;
		boolean yy_initial = true;
		int yy_this_accept;

		yy_mark_start();
		yy_this_accept = yy_acpt[yy_state];
		if (YY_NOT_ACCEPT != yy_this_accept) {
			yy_last_accept_state = yy_state;
			yy_mark_end();
		}
		while (true) {
			if (yy_initial && yy_at_bol) yy_lookahead = YY_BOL;
			else yy_lookahead = yy_advance();
			yy_next_state = YY_F;
			yy_next_state = yy_nxt[yy_rmap[yy_state]][yy_cmap[yy_lookahead]];
			if (YY_EOF == yy_lookahead && true == yy_initial) {

    switch(yy_lexical_state) {
    case YYINITIAL:
	break;
    case COMMENT:
	yybegin(YYINITIAL);
	return new Symbol(TokenConstants.ERROR, "Unmatched *)");
    case LINE_COMMENT:
	break;
    case STRING:
	yybegin(YYINITIAL);
	return new Symbol(TokenConstants.ERROR, "EOF in string constant");
    case STRING_RECOVER:
	break;
    }
    return new Symbol(TokenConstants.EOF);
			}
			if (YY_F != yy_next_state) {
				yy_state = yy_next_state;
				yy_initial = false;
				yy_this_accept = yy_acpt[yy_state];
				if (YY_NOT_ACCEPT != yy_this_accept) {
					yy_last_accept_state = yy_state;
					yy_mark_end();
				}
			}
			else {
				if (YY_NO_STATE == yy_last_accept_state) {
					throw (new Error("Lexical Error: Unmatched Input."));
				}
				else {
					yy_anchor = yy_acpt[yy_last_accept_state];
					if (0 != (YY_END & yy_anchor)) {
						yy_move_end();
					}
					yy_to_mark();
					switch (yy_last_accept_state) {
					case 1:
						
					case -2:
						break;
					case 2:
						{ curr_lineno++; }
					case -3:
						break;
					case 3:
						{}
					case -4:
						break;
					case 4:
						{ return new Symbol(TokenConstants.MINUS); }
					case -5:
						break;
					case 5:
						{ return new Symbol(TokenConstants.ERROR, yytext()); }
					case -6:
						break;
					case 6:
						{ return new Symbol(TokenConstants.LPAREN); }
					case -7:
						break;
					case 7:
						{ return new Symbol(TokenConstants.MULT); }
					case -8:
						break;
					case 8:
						{ return new Symbol(TokenConstants.RPAREN); }
					case -9:
						break;
					case 9:
						{ return new Symbol(TokenConstants.EQ); }
					case -10:
						break;
					case 10:
						{ return new Symbol(TokenConstants.LT); }
					case -11:
						break;
					case 11:
						{ return new Symbol(TokenConstants.INT_CONST,
					    AbstractTable.inttable.addString(yytext())); }
					case -12:
						break;
					case 12:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -13:
						break;
					case 13:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -14:
						break;
					case 14:
						{ return new Symbol(TokenConstants.PLUS); }
					case -15:
						break;
					case 15:
						{ return new Symbol(TokenConstants.DIV); }
					case -16:
						break;
					case 16:
						{ return new Symbol(TokenConstants.DOT); }
					case -17:
						break;
					case 17:
						{ return new Symbol(TokenConstants.NEG); }
					case -18:
						break;
					case 18:
						{ return new Symbol(TokenConstants.COMMA); }
					case -19:
						break;
					case 19:
						{ return new Symbol(TokenConstants.SEMI); }
					case -20:
						break;
					case 20:
						{ return new Symbol(TokenConstants.COLON); }
					case -21:
						break;
					case 21:
						{ return new Symbol(TokenConstants.AT); }
					case -22:
						break;
					case 22:
						{ return new Symbol(TokenConstants.RBRACE); }
					case -23:
						break;
					case 23:
						{ return new Symbol(TokenConstants.LBRACE); }
					case -24:
						break;
					case 24:
						{ string_buf.setLength(0); yybegin(STRING); }
					case -25:
						break;
					case 25:
						{ yybegin(LINE_COMMENT); }
					case -26:
						break;
					case 26:
						{ yybegin(COMMENT);
			  comment_nesting++;
			}
					case -27:
						break;
					case 27:
						{ /* error - unmatched comment */
			  return new Symbol(TokenConstants.ERROR, "Unmatched *)");
                	}
					case -28:
						break;
					case 28:
						{ return new Symbol(TokenConstants.DARROW); }
					case -29:
						break;
					case 29:
						{ return new Symbol(TokenConstants.ASSIGN); }
					case -30:
						break;
					case 30:
						{ return new Symbol(TokenConstants.LE); }
					case -31:
						break;
					case 31:
						{ return new Symbol(TokenConstants.FI); }
					case -32:
						break;
					case 32:
						{ return new Symbol(TokenConstants.IF); }
					case -33:
						break;
					case 33:
						{ return new Symbol(TokenConstants.IN); }
					case -34:
						break;
					case 34:
						{ return new Symbol(TokenConstants.OF); }
					case -35:
						break;
					case 35:
						{ return new Symbol(TokenConstants.LET); }
					case -36:
						break;
					case 36:
						{ return new Symbol(TokenConstants.NEW); }
					case -37:
						break;
					case 37:
						{ return new Symbol(TokenConstants.NOT); }
					case -38:
						break;
					case 38:
						{ return new Symbol(TokenConstants.CASE); }
					case -39:
						break;
					case 39:
						{ return new Symbol(TokenConstants.ESAC); }
					case -40:
						break;
					case 40:
						{ return new Symbol(TokenConstants.ELSE); }
					case -41:
						break;
					case 41:
						{ return new Symbol(TokenConstants.LOOP); }
					case -42:
						break;
					case 42:
						{ return new Symbol(TokenConstants.THEN); }
					case -43:
						break;
					case 43:
						{ return new Symbol(TokenConstants.POOL); }
					case -44:
						break;
					case 44:
						{ return new Symbol(TokenConstants.BOOL_CONST, Boolean.TRUE); }
					case -45:
						break;
					case 45:
						{ return new Symbol(TokenConstants.CLASS); }
					case -46:
						break;
					case 46:
						{ return new Symbol(TokenConstants.BOOL_CONST, Boolean.FALSE); }
					case -47:
						break;
					case 47:
						{ return new Symbol(TokenConstants.WHILE); }
					case -48:
						break;
					case 48:
						{ return new Symbol(TokenConstants.ISVOID); }
					case -49:
						break;
					case 49:
						{ return new Symbol(TokenConstants.INHERITS); }
					case -50:
						break;
					case 50:
						{ curr_lineno++; }
					case -51:
						break;
					case 51:
						{}
					case -52:
						break;
					case 52:
						{}
					case -53:
						break;
					case 53:
						{}
					case -54:
						break;
					case 54:
						{ comment_nesting++; }
					case -55:
						break;
					case 55:
						{ comment_nesting--;
                          if (comment_nesting == 0) yybegin(YYINITIAL); }
					case -56:
						break;
					case 56:
						{}
					case -57:
						break;
					case 57:
						{ curr_lineno++; yybegin(YYINITIAL); }
					case -58:
						break;
					case 58:
						{}
					case -59:
						break;
					case 59:
						{ curr_lineno++;
                          yybegin(YYINITIAL);
			  return new Symbol(TokenConstants.ERROR,
					    "Unterminated string constant"); }
					case -60:
						break;
					case 60:
						{ // sm: previously, the regexp here was [^\\\n\"\0]+
                          // the problem is that inside [] jlex doesn't recognize
                          // metacharacters like \n or \0, so this doesn't do the
                          // right thing.  so while it's inefficient, a correct
                          // implementation is obtained with '.'
                          Symbol res = check_string_overflow();
                          if (res != null) return res;
                          string_buf.append(yytext()); }
					case -61:
						break;
					case 61:
						{ yybegin(YYINITIAL);
                          Symbol res = check_string_overflow();
			  if (res != null) return res;
			  return new Symbol(TokenConstants.STR_CONST,
				            AbstractTable.stringtable.addString(string_buf.toString()));
                        }
					case -62:
						break;
					case 62:
						{ yybegin(YYINITIAL);
			  return new Symbol(TokenConstants.ERROR,
					    "backslash at end of file"); }
					case -63:
						break;
					case 63:
						{ yybegin(STRING_RECOVER);
			  return new Symbol(TokenConstants.ERROR,
					    "String contains null character");}
					case -64:
						break;
					case 64:
						{ curr_lineno++;
                          Symbol res = check_string_overflow();
                          if (res != null) return res;
                          string_buf.append('\n'); }
					case -65:
						break;
					case 65:
						{ Symbol res = check_string_overflow();
                          if (res != null) return res;
                          // sm: fixed off-by-one bug: was (1,1)
                          string_buf.append(yytext().substring(1, 2)); }
					case -66:
						break;
					case 66:
						{ Symbol res = check_string_overflow();
                          if (res != null) return res;
                          string_buf.append('\f'); }
					case -67:
						break;
					case 67:
						{ Symbol res = check_string_overflow();
                          if (res != null) return res;
                          string_buf.append('\n'); }
					case -68:
						break;
					case 68:
						{ Symbol res = check_string_overflow();
                          if (res != null) return res;
                          string_buf.append('\t'); }
					case -69:
						break;
					case 69:
						{ Symbol res = check_string_overflow();
                          if (res != null) return res;
                          string_buf.append('\b'); }
					case -70:
						break;
					case 70:
						{ yybegin(STRING_RECOVER);
                          return new Symbol(TokenConstants.ERROR,
					    "String contains escaped null character"); }
					case -71:
						break;
					case 71:
						{ curr_lineno++; yybegin(YYINITIAL); }
					case -72:
						break;
					case 72:
						{}
					case -73:
						break;
					case 73:
						{ yybegin(YYINITIAL); }
					case -74:
						break;
					case 74:
						{}
					case -75:
						break;
					case 75:
						{ curr_lineno++; }
					case -76:
						break;
					case 76:
						{}
					case -77:
						break;
					case 78:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -78:
						break;
					case 79:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -79:
						break;
					case 80:
						{ return new Symbol(TokenConstants.FI); }
					case -80:
						break;
					case 81:
						{ return new Symbol(TokenConstants.IF); }
					case -81:
						break;
					case 82:
						{ return new Symbol(TokenConstants.IN); }
					case -82:
						break;
					case 83:
						{ return new Symbol(TokenConstants.OF); }
					case -83:
						break;
					case 84:
						{ return new Symbol(TokenConstants.LET); }
					case -84:
						break;
					case 85:
						{ return new Symbol(TokenConstants.NEW); }
					case -85:
						break;
					case 86:
						{ return new Symbol(TokenConstants.NOT); }
					case -86:
						break;
					case 87:
						{ return new Symbol(TokenConstants.CASE); }
					case -87:
						break;
					case 88:
						{ return new Symbol(TokenConstants.ESAC); }
					case -88:
						break;
					case 89:
						{ return new Symbol(TokenConstants.ELSE); }
					case -89:
						break;
					case 90:
						{ return new Symbol(TokenConstants.LOOP); }
					case -90:
						break;
					case 91:
						{ return new Symbol(TokenConstants.THEN); }
					case -91:
						break;
					case 92:
						{ return new Symbol(TokenConstants.POOL); }
					case -92:
						break;
					case 93:
						{ return new Symbol(TokenConstants.CLASS); }
					case -93:
						break;
					case 94:
						{ return new Symbol(TokenConstants.WHILE); }
					case -94:
						break;
					case 95:
						{ return new Symbol(TokenConstants.ISVOID); }
					case -95:
						break;
					case 96:
						{ return new Symbol(TokenConstants.INHERITS); }
					case -96:
						break;
					case 97:
						{}
					case -97:
						break;
					case 99:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -98:
						break;
					case 100:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -99:
						break;
					case 102:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -100:
						break;
					case 103:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -101:
						break;
					case 104:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -102:
						break;
					case 105:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -103:
						break;
					case 106:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -104:
						break;
					case 107:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -105:
						break;
					case 108:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -106:
						break;
					case 109:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -107:
						break;
					case 110:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -108:
						break;
					case 111:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -109:
						break;
					case 112:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -110:
						break;
					case 113:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -111:
						break;
					case 114:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -112:
						break;
					case 115:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -113:
						break;
					case 116:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -114:
						break;
					case 117:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -115:
						break;
					case 118:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -116:
						break;
					case 119:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -117:
						break;
					case 120:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -118:
						break;
					case 121:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -119:
						break;
					case 122:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -120:
						break;
					case 123:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -121:
						break;
					case 124:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -122:
						break;
					case 125:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -123:
						break;
					case 126:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -124:
						break;
					case 127:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -125:
						break;
					case 128:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -126:
						break;
					case 129:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -127:
						break;
					case 130:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -128:
						break;
					case 131:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -129:
						break;
					case 132:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -130:
						break;
					case 133:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -131:
						break;
					case 134:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -132:
						break;
					case 135:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -133:
						break;
					case 136:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -134:
						break;
					case 137:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -135:
						break;
					case 138:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -136:
						break;
					case 139:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -137:
						break;
					case 140:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -138:
						break;
					case 141:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -139:
						break;
					case 142:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -140:
						break;
					case 143:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -141:
						break;
					case 144:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -142:
						break;
					case 145:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -143:
						break;
					case 146:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -144:
						break;
					case 147:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -145:
						break;
					case 148:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -146:
						break;
					case 149:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -147:
						break;
					case 150:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -148:
						break;
					case 151:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -149:
						break;
					case 152:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -150:
						break;
					case 153:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -151:
						break;
					case 154:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -152:
						break;
					case 155:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -153:
						break;
					case 156:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -154:
						break;
					case 157:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -155:
						break;
					case 158:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -156:
						break;
					case 159:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -157:
						break;
					case 160:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -158:
						break;
					case 161:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -159:
						break;
					case 162:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -160:
						break;
					case 163:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -161:
						break;
					case 164:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -162:
						break;
					case 165:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -163:
						break;
					case 166:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -164:
						break;
					case 167:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -165:
						break;
					case 168:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -166:
						break;
					case 169:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -167:
						break;
					case 170:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -168:
						break;
					case 171:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -169:
						break;
					case 172:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -170:
						break;
					case 173:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -171:
						break;
					case 174:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -172:
						break;
					case 175:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -173:
						break;
					case 176:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -174:
						break;
					case 177:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -175:
						break;
					case 178:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -176:
						break;
					case 179:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -177:
						break;
					case 180:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -178:
						break;
					case 181:
						{ return new Symbol(TokenConstants.OBJECTID,
					    AbstractTable.idtable.addString(yytext())); }
					case -179:
						break;
					case 182:
						{ return new Symbol(TokenConstants.TYPEID,
					    AbstractTable.idtable.addString(yytext())); }
					case -180:
						break;
					default:
						yy_error(YY_E_INTERNAL,false);
					case -1:
					}
					yy_initial = true;
					yy_state = yy_state_dtrans[yy_lexical_state];
					yy_next_state = YY_NO_STATE;
					yy_last_accept_state = YY_NO_STATE;
					yy_mark_start();
					yy_this_accept = yy_acpt[yy_state];
					if (YY_NOT_ACCEPT != yy_this_accept) {
						yy_last_accept_state = yy_state;
						yy_mark_end();
					}
				}
			}
		}
	}
}
