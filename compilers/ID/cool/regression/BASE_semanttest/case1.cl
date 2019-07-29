class MyClass is
  do(arg:Object) : Void is
	case arg of
	  o1 : Object1 => 1;
	  o2 : Object2 => o2.special();
	esac 
  end;
end ;

