class C1 is end;

class C2 inherits C1 is end;

class C3 inherits C1 is end;

class Main is
   main() : Int is if true then 1 else 2 fi end;

   other() : C1 is if true then new C2 else new C3 fi end;
end;   
