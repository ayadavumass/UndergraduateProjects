class Main is
  x : Object;
  main() : String is
	case mkobj(self) of
	   a : Int => 1.abort();
	   a : String => "".abort();
	   a : Bool => true.abort();
	   o : Object => o.abort()
	esac
  end;

  mkobj(x : Object) : Object is
	x
  end;
end;
