-- new SELF_TYPE creates an object with the same dynamic type as self,
-- which affects initialization of the new object's attributes.


class Base inherits IO
{
  baseAttr : Int <- report( 1 );

  report( value : Int ) : Int
  {
    {
      out_int( value );
      out_string( "\n" );
      value;
    }
  };

  duplicate() : Base
  {
    new SELF_TYPE
  };
};


class Derived inherits Base
{
  derivedAttr : Int <- report( 2 );
};


class Main 
{
  main() : Object
  {
    (new Derived).duplicate()
  };
};
