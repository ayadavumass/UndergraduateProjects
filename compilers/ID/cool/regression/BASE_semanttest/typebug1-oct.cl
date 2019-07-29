class A is end;

class D inherits Int is end;

class E inherits F is end;

class B inherits C is end;

class C inherits B is end;

class A is end;

class SELF_TYPE inherits A is end;

class SELF_TYPE inherits SELF_TYPE is end;
