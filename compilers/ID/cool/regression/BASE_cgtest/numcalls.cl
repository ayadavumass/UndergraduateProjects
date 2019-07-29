class Main {
  f1(p:Object):Object {{
    if false then
      p <- new Object     -- might think we have a P statically
    else
      0
    fi;
    p.copy();         -- then call it
  }};

  main():Object {
    f1(3)        -- when it's really a C
  };
};
