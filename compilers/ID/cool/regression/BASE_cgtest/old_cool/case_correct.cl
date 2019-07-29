class Main is
  x : Object;

  main() : String is
	case x of
	   a : Int => "int";
	   a : String => "string";
	   a : Bool => "bool"
	esac
  end;

  mkint(x : Object) : Int is
	case x of
	   a : Int => a
	esac
  end;
end;
