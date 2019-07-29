Class A is
	foo() : SELF_TYPE is self end;
end;

Class B inherits A is
	foo() : B is new B end;
end;

Class Main inherits IO is
	main():Object is 0 end;
end;
