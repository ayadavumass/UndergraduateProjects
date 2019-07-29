/*
Copyright (c) 2000 The Regents of the University of California.
All rights reserved.

Permission to use, copy, modify, and distribute this software for any
purpose, without fee, and without written agreement is hereby granted,
provided that the above copyright notice and the following two
paragraphs appear in all copies of this software.

IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF
CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
*/

import java.io.InputStreamReader;
import java.io.IOException;
import java_cup.runtime.Symbol;

/** The lexer driver class for token lexer */
class TokenLexer {

    /** Loops over lexed tokens, printing them out to the console */
    public static void main(String[] args) {
	try {
	    CoolTokenLexer lexer = new CoolTokenLexer(new InputStreamReader(System.in));
	    Symbol s;
	    while ((s = lexer.next_token()).sym != TokenConstants.EOF) {
		Utilities.dumpToken(System.out, lexer.curr_lineno(), s);
	    }
	} catch (IOException ex) {
	    Utilities.fatalError("Unexpected exception in lexer");
	}
    }
}

