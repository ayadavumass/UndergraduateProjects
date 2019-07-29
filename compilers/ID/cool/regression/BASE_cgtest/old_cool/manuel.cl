(*
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
*)
Class A is
	a : Int;
	double() : Int is a*2 end;
	triple() : Int is a*3 end;
end;


Class B inherits A is
	b : Int;
	triple() : Int is (a+b)*3 end;
end;

Class Main is
   main(): Int is 1 end;
end;



