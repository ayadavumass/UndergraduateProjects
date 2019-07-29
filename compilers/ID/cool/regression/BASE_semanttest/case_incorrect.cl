class Classy is
  do(arg:Object) : Object is
	case arg of
	  o1 : Object1 => 1;
	  o2 : Object2 => o2.special();
	esac 
  end;
end ;

class SubClass inherits Classy is end;

class SubSubClass inherits SubClass is end;

class SubSubSubClass inherits SubSubClass is end;

class Cased is
   f(x : SubSubClass) : Int is
	case x of
	   a : Classy => 1;
	   a : SubClass => 2;
	   a : SubSubClass => 3;
	   a : SubSubSubClass => 4;
	   a : SubClass => 5;
	esac
   end;
end;

class Main is
  main(x: Object) : String is
	case x of
	   a : Int => "int";
	   a : SELF_TYPE => "string";
	   self : Bool => "bool";
	esac
  end;
end;
