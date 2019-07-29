Class C1 inherits SELF_TYPE is 
  f(x : Int, y : SELF_TYPE) : Int is 1 end;

end;

Class SELF_TYPE is
  b : SELF_TYPE <- new SELF_TYPE;

  g(x : Int) : SELF_TYPE is
	let y : SELF_TYPE in
	  case y of
	    a : SELF_TYPE => new SELF_TYPE;
	    a : Int => (new SELF_TYPE)@SELF_TYPE.g(a);
	  esac
	end
  end;
end;


