class Crash is
   car : IO;
   head() : IO is car end;
end;

class Burn inherits Crash is end;

class Main is
   main() : Object is
      new Burn.head().type_name()
   end;
end;
