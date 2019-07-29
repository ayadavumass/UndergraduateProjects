Class Object is 
	copy() : SELF_TYPE is self end;
	isProto() : Bool is false end;
end;

Class Int is 
	val : Prim__Int;
end;

Class Bool is 
	val : Prim__Int;
end;

Class String is
	val : Prim__String;
end;

Class A is
	a : Int <- 123;
	b : Int <- 55555;
	c : Int <- 0;
	d : Int <- 987654321987654321;
end;


Class B inherits A is
	u : String <- "Test\n\t";
	v : String <- "Fun with spaces";
	w : String <- "";
	x : String <- "123";
end;

Class C inherits B is
	y : Bool <- not false;
	z : Bool <- not true;
end;

Class D inherits C is
	r : Int <- ~a;
end;

