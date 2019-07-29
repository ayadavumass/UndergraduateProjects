-- String literals must be translated into static string instances,
-- even if the literal also happens to be used internally by the
-- compiler for other purposes.

-- (In other words, this is a malicious little test program designed
-- to catch people who hard-wired things like "_no_class" as
-- superfluous strings that should not be emitted with the rest of the
-- string table.)


class Main inherits IO
{
  show( text : String ) : Object
  {
    {
      out_string( text );
      out_string( "\n" );
    }
  };

  main() : Object
  {
    {
      show( "no_class" );
      show( "_no_class" );
      show( "__no_class" );
      show( "prim_slot" );
      show( "_prim_slot" );
      show( "__prim_slot" );
      show( "SELF_TYPE" );
      show( "_SELF_TYPE" );
      show( "__SELF_TYPE" );
    }
  };
};
